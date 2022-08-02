#include "uwc_event.h"

uwcEvent_t uwc_event_udp_init(void) {
  while (!uwcIsWifiInit) {
    ESP_LOGW(uwc_tag_event,
             "WiFi is not initialized yet, initializing wifi...");
    uwc_event_wifi_init();
    if (!uwcEventErr) {
      break;
    }
  }

  if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &uwcIPInfo)) {
    ESP_LOGE(uwc_tag_event, "Error getting IP info!");
    return;
  }

  char gw[16];
  sprintf(gw, IPSTR, IP2STR(&uwcIPInfo.gw));
  strcpy(SERV_IP, gw);
  SERV_PORT = 8888;

  uwcEventErr = uwc_udp_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}