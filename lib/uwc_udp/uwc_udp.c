#include "uwc_udp.h"

char SERV_IP[16] = "192.168.43.1";
u16_t SERV_PORT = 8888;

static char uwcUdpBufRX[UDP_BUF_SIZE];
static char uwcUdpBufTX[UDP_BUF_SIZE];

static int sock = 0;
static bool isHandled = false;

struct sockaddr_in destAddr;
struct sockaddr_storage sourceAddr;
struct timeval timeout;
static socklen_t socklen = sizeof(sourceAddr);

void uwc_udp_set_timeout(u8_t sec, u8_t usec) {
  timeout.tv_sec = sec;
  timeout.tv_usec = usec;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
}

esp_err_t uwc_udp_init() {
  if (!uwcIsWifiInit) {
    return ESP_FAIL;
  }

  destAddr.sin_addr.s_addr = inet_addr(SERV_IP);
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = htons(SERV_PORT);

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sock < 0) {
    ESP_LOGE(uwc_tag_udp, "Unable to create socket: errno %d", errno);
    return ESP_FAIL;
  }
  uwc_udp_set_timeout(1, 0);
  uwc_udp_flush();

  ESP_LOGI(uwc_tag_udp, "Socket created, sending SYN to %s:%d", SERV_IP,
           SERV_PORT);
  do {  // Handshaking...
    uwc_udp_send("SYN\n");
    uwc_udp_recv();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  } while (!uwc_udp_is_data_match("ACK\n"));

  ESP_LOGI(uwc_tag_udp, "ACK received!");
  ESP_LOGI(uwc_tag_udp, "UDP has been initialized!");
  return ESP_OK;
}

esp_err_t uwc_udp_send(char* data) {
  if (!uwcIsWifiInit) {
    return ESP_FAIL;
  }

  strcpy(uwcUdpBufTX, data);
  int err = sendto(sock, uwcUdpBufTX, strlen(uwcUdpBufTX), 0,
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

  int uwcUdpBufRXLen = recvfrom(sock, uwcUdpBufRX, sizeof(uwcUdpBufRX) - 1, 0,
                                (struct sockaddr*)&sourceAddr, &socklen);

  if (uwcUdpBufRXLen < 0) {
    // if (errno != 11) {  // print error only if error is not timeout error.
    //   ESP_LOGE(uwc_tag_udp, "recvfrom failed: errno %d", errno);
    // }
    return 0;
  }
  uwcUdpBufRX[uwcUdpBufRXLen] = 0;  // insert null-terminated string at end
  // because C string format rules.
  isHandled = false;
  return uwcUdpBufRXLen;
}

char* uwc_udp_get_data(void) {
  isHandled = true;
  return &uwcUdpBufRX[0];
}

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
  ESP_LOGI(uwc_tag_udp, "TX : %s (%i) %c", uwcUdpBufTX, strlen(uwcUdpBufTX),
           uwcUdpBufTX[5]);
  ESP_LOGI(uwc_tag_udp, "RX : %s (%i) %c", uwcUdpBufRX, strlen(uwcUdpBufRX),
           uwcUdpBufRX[5]);
}