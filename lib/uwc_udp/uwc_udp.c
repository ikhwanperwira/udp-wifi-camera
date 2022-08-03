#include "uwc_udp.h"

char SERV_IPV4[16] = "192.168.43.1";
u16_t SERV_PORT = 8888;
bool interruptHandshake = false;
u8_t timeoutCounter = 1;
bool isUdpInit = false;

static u16_t CLNT_PORT = 9999;

static char uwcUdpBufRX[UDP_BUF_SIZE];
static char uwcUdpBufTX[UDP_BUF_SIZE];

static int uwcSock = 0;
static bool isHandled = false;

struct sockaddr_in destAddr;
struct sockaddr_in clientAddr;
struct sockaddr_storage sourceAddr;
struct timeval timeout;
static socklen_t socklen = sizeof(sourceAddr);

void uwc_udp_handshake() {
  interruptHandshake = false;
  ESP_LOGW(uwc_tag_udp, "Sending SYN to %s:%d", SERV_IPV4, SERV_PORT);
  do {  // Handshaking...
    timeoutCounter = 1;
    uwc_udp_send("SYN\n");
    uwc_udp_recv();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (interruptHandshake) {
      break;
    }
  } while (!uwc_udp_is_data_match("ACK\n"));
}

void uwc_udp_set_timeout(u8_t sec, u8_t usec) {
  timeout.tv_sec = sec;
  timeout.tv_usec = usec;
  setsockopt(uwcSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
}

esp_err_t uwc_udp_init() {
  if (!uwcIsWifiInit) {
    return ESP_FAIL;
  }

  close(uwcSock);

  isUdpInit = false;

  destAddr.sin_addr.s_addr = inet_addr(SERV_IPV4);
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = htons(SERV_PORT);

  clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port = htons(CLNT_PORT);

  uwcSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (uwcSock < 0) {
    ESP_LOGE(uwc_tag_udp, "Unable to create socket: errno %d", errno);
    return ESP_FAIL;
  }

  uwc_udp_set_timeout(1, 0);
  int opt = 1;
  setsockopt(uwcSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int err = bind(uwcSock, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
  if (err < 0) {
    ESP_LOGE(uwc_tag_udp, "Socket unable to bind: errno %d", errno);
  }
  ESP_LOGI(uwc_tag_udp, "Socket bound, port %d", 9999);

  uwc_udp_flush();
  uwc_udp_handshake();

  isUdpInit = true;

  ESP_LOGI(uwc_tag_udp, "ACK received!");
  ESP_LOGI(uwc_tag_udp, "UDP has been initialized!");
  return ESP_OK;
}

esp_err_t uwc_udp_send(char* data) {
  if (!uwcIsWifiInit) {
    return ESP_FAIL;
  }

  strcpy(uwcUdpBufTX, data);
  int err = sendto(uwcSock, uwcUdpBufTX, strlen(uwcUdpBufTX), 0,
                   (struct sockaddr*)&destAddr, sizeof(destAddr));
  if (err < 0) {
    ESP_LOGE(uwc_tag_udp, "Error occurred during sending: errno %d", errno);
    return ESP_FAIL;
  }
  return ESP_OK;
}

int uwc_udp_recv(void) {
  if (!uwcIsWifiInit) {
    return 0;
  }

  int uwcUdpBufRXLen = recvfrom(uwcSock, uwcUdpBufRX, sizeof(uwcUdpBufRX) - 1,
                                0, (struct sockaddr*)&sourceAddr, &socklen);

  if (uwcUdpBufRXLen < 0) {
    // ESP_LOGE(uwc_tag_udp, "recvfrom failed: errno %d", errno);
    timeoutCounter++;
    ESP_LOGW(uwc_tag_udp, "Timeout Counter: %i", timeoutCounter);
    return 0;
  }
  timeoutCounter = 1;
  uwcUdpBufRX[uwcUdpBufRXLen] = 0;  // insert null-terminated string at end
  // because C string format rules.
  isHandled = false;  // for event handling.
  return uwcUdpBufRXLen;
}

char* uwc_udp_get_data(void) { return &uwcUdpBufRX[0]; }

bool uwc_udp_is_data_match(char* check) {
  return (bool)!strcmp(check, uwcUdpBufRX);
}

void uwc_udp_on(char* data, void on_match(), void on_unmatch()) {
  if (isHandled) {  // return if another event already handled this data.
    return;
  }
  if (!strcmp(data, uwcUdpBufRX)) {
    if (on_match) {
      on_match();
      isHandled = true;
    }
    return;
  } else {
    if (on_unmatch) {
      on_unmatch();
      isHandled = true;
    }
    return;
  }
}

void uwc_udp_flush(void) {
  memset(uwcUdpBufTX, 0, UDP_BUF_SIZE);
  memset(uwcUdpBufRX, 0, UDP_BUF_SIZE);
  ESP_LOGI(uwc_tag_udp, "UDP buffer has been flushed!");
}

void uwc_udp_debug(void) {
  ESP_LOGI(uwc_tag_udp, "TX : |%s| (%i) TX[4]=%c", uwcUdpBufTX,
           strlen(uwcUdpBufTX), uwcUdpBufTX[4]);
  ESP_LOGI(uwc_tag_udp, "RX : |%s| (%i) RX[4]=%c", uwcUdpBufRX,
           strlen(uwcUdpBufRX), uwcUdpBufRX[4]);
}

void uwc_udp_input(char* msg, char* val_out, bool show_echo, bool remove_eol) {
  timeoutCounter = 1;
  if (uwc_udp_send(msg)) {
    ESP_LOGE(uwc_tag_udp, "Unable to send echo!");
    return;
  }
  for (;;) {
    if (timeoutCounter == 30) {
      uwc_udp_send("\ninput timeout!\n");
      return;
    }
    if (uwc_udp_recv() > 0) {  // check if data incoming!
      strcpy(val_out, uwc_udp_get_data());
      break;
    }
  }
  if (remove_eol) {
    uwc_eol_remover(val_out);
  }
  if (show_echo) {
    uwc_udp_send(val_out);
    uwc_udp_send("\n");
  }
}
