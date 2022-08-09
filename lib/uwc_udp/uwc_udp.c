#include "uwc_udp.h"

static char uwcUdpBufRX[UDP_BUF_SIZE];
static char uwcUdpBufTX[UDP_BUF_SIZE];
static int uwcUdpSock = 0;
static bool uwcUdpIsHandled = false;

struct sockaddr_in destAddr;
struct sockaddr_in clientAddr;
struct sockaddr_storage sourceAddr;
static socklen_t sourceAddrLen = sizeof(sourceAddr);
struct timeval uwcUdpTimeout;

char SERV_IPV4[16] = "192.168.43.1";
u16_t SERV_PORT = 39876;
u16_t CLNT_PORT = 9999;
bool uwcUdpIsInit = false;
u8_t uwcUdpTimeoutCount = 1;

void uwc_udp_handshake() {
  ESP_LOGW(uwc_tag_udp, "Sending SYN to %s:%d", SERV_IPV4, SERV_PORT);
  do {  // Handshaking...
    uwc_udp_send("SYN\n");
    uwc_udp_recv();
  } while (!uwc_udp_is_data_match("ACK\n"));
}

void uwc_udp_set_timeout(u8_t sec, u8_t usec) {
  uwcUdpTimeoutCount = 1;
  uwcUdpTimeout.tv_sec = sec;
  uwcUdpTimeout.tv_usec = usec;
  setsockopt(uwcUdpSock, SOL_SOCKET, SO_RCVTIMEO, &uwcUdpTimeout,
             sizeof uwcUdpTimeout);
}

esp_err_t uwc_udp_init() {
  if (!uwcWifiIsInit) {
    return ESP_FAIL;
  }

  close(uwcUdpSock);

  uwcUdpIsInit = false;

  destAddr.sin_addr.s_addr = inet_addr(SERV_IPV4);
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = htons(SERV_PORT);

  clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port = htons(CLNT_PORT);

  uwcUdpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (uwcUdpSock < 0) {
    ESP_LOGE(uwc_tag_udp, "Unable to create socket: errno %d", errno);
    return ESP_FAIL;
  }

  uwc_udp_set_timeout(1, 0);
  int opt = 1;
  setsockopt(uwcUdpSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(uwcUdpSock, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
    ESP_LOGE(uwc_tag_udp, "Socket unable to bind: errno %d", errno);
    return ESP_FAIL;
  }
  ESP_LOGI(uwc_tag_udp, "Socket bound, port %d", 9999);

  uwc_udp_flush();
  uwc_udp_handshake();
  uwcUdpIsInit = true;

  ESP_LOGI(uwc_tag_udp, "ACK received!");
  ESP_LOGI(uwc_tag_udp, "UDP has been initialized!");
  return ESP_OK;
}

esp_err_t uwc_udp_send(char* data) {
  uwcUdpTimeoutCount = 1;
  if (!uwcWifiIsInit) {
    return ESP_FAIL;
  }

  strcpy(uwcUdpBufTX, data);
  if (sendto(uwcUdpSock, uwcUdpBufTX, strlen(uwcUdpBufTX), 0,
             (struct sockaddr*)&destAddr, sizeof(destAddr)) < 0) {
    ESP_LOGE(uwc_tag_udp, "Error occurred during sending: errno %d", errno);
    return ESP_FAIL;
  }
  return ESP_OK;
}

int uwc_udp_recv(void) {
  if (!uwcWifiIsInit) {
    return 0;
  }

  int uwcUdpBufRXLen =
      recvfrom(uwcUdpSock, uwcUdpBufRX, sizeof(uwcUdpBufRX) - 1, 0,
               (struct sockaddr*)&sourceAddr, &sourceAddrLen);

  if (uwcUdpBufRXLen < 0) {
    // ESP_LOGE(uwc_tag_udp, "recvfrom failed: errno %d", errno);
    if (uwcUdpTimeout.tv_sec != 0) uwcUdpTimeoutCount++;
    // ESP_LOGW(uwc_tag_udp, "Timeout Counter: %i", uwcUdpTimeoutCount);
    return 0;
  }
  uwcUdpTimeoutCount = 1;
  uwcUdpBufRX[uwcUdpBufRXLen] = 0;  // insert null-terminated string at end
  // because C string format rules.
  uwcUdpIsHandled = false;  // for event handling.
  return uwcUdpBufRXLen;
}

char* uwc_udp_get_data(void) { return &uwcUdpBufRX[0]; }

bool uwc_udp_is_data_match(char* check) {
  return (bool)!strcmp(check, uwcUdpBufRX);
}

void uwc_udp_on(char* data, void on_match(), void on_unmatch()) {
  if (uwcUdpIsHandled) {  // return if another event already handled this data.
    return;
  }

  if (!strcmp(data, uwcUdpBufRX)) {
    if (on_match) {
      on_match();
      uwcUdpIsHandled = true;
    }
    return;
  } else {
    if (on_unmatch) {
      on_unmatch();
      uwcUdpIsHandled = true;
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
  uwcUdpTimeoutCount = 1;
  if (uwc_udp_send(msg)) {
    ESP_LOGE(uwc_tag_udp, "Unable to send echo!");
    return;
  }
  for (;;) {
    if (uwcUdpTimeoutCount % 30 == 0) {
      uwc_udp_send("\ninput timeout!\n");
      return;
    }
    if (uwc_udp_recv() > 0) {  // check if data is incoming!
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

ssize_t uwc_udp_send_raw(const void* data, size_t len) {
  return sendto(uwcUdpSock, data, len, 0, (struct sockaddr*)&destAddr,
                sizeof(destAddr));
}
