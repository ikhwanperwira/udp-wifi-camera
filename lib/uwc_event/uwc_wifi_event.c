#include "uwc_event.h"

uwcEvent_t uwc_event_wifi_init() {
  if (uwcWifiIsInit) {
    ESP_LOGW(uwc_tag_event, "WiFi already initialized!");
    return;
  }

  uwc_event_nvs_init();
  uwc_event_nvs_open();
  uwc_event_nvs_read_ssid();
  uwc_event_nvs_read_pasw();
  uwcEventErr = uwc_wifi_init_sta();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_wifi_deinit(void) {
  uwcEventErr = uwc_wifi_deinit_sta();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_wifi_info(void) {
  if (!uwcWifiIsInit) {
    ESP_LOGE(uwc_tag_event, "WiFI is not initialized!");
    return;
  }

  if (!tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &uwcWifiIpInfo)) {
    ESP_LOGI(uwc_tag_event, "IPV4 -> " IPSTR, IP2STR(&uwcWifiIpInfo.ip));
    ESP_LOGI(uwc_tag_event, "GTWY -> " IPSTR, IP2STR(&uwcWifiIpInfo.gw));
    ESP_LOGI(uwc_tag_event, "NETM -> " IPSTR, IP2STR(&uwcWifiIpInfo.netmask));
    ESP_LOGI(uwc_tag_event, "SSID -> %s", WIFI_SSID);
    ESP_LOGI(uwc_tag_event, "PASW -> %s", WIFI_PASW);
  }
}

uwcEvent_t uwc_event_wifi_setup_with_uart(void) {
  char confirm[4];
  uwc_uart_input("Enter SSID: ", WIFI_SSID, true, true);
  uwc_uart_input("Enter PASW: ", WIFI_PASW, true, true);
  uwc_uart_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    ESP_LOGW(uwc_tag_event, "WiFi setup aborted!");
    return;
  }

  uwc_event_nvs_write_ssid();
  uwc_event_nvs_write_pasw();
  uwc_event_nvs_commit_auth();
  if (strlen((char *)WIFI_PASW) < 8) {
    ESP_LOGW(uwc_tag_event,
             "Warning! Password length is less than 8 characters!");
  }
  uwc_uart_send("WiFi setup has been updated!\n");
}

uwcEvent_t uwc_event_wifi_setup_with_udp(void) {
  char confirm[4];
  uwc_udp_input("Enter SSID: ", WIFI_SSID, true, true);
  uwc_udp_input("Enter PASW: ", WIFI_PASW, true, true);
  uwc_udp_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    ESP_LOGW(uwc_tag_event, "WiFi setup aborted!");
    return;
  }

  uwc_event_nvs_write_ssid();
  uwc_event_nvs_write_pasw();
  uwc_event_nvs_commit_auth();
  if (strlen((char *)WIFI_PASW) < 8) {
    ESP_LOGW(uwc_tag_event,
             "Warning! Password length is less than 8 characters!");
    uwc_udp_send("Warning! Password length is less than 8 characters!");
  }
  uwc_udp_send("WiFi setup has been updated!\n");
}
