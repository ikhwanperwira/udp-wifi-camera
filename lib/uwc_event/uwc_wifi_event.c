#include "uwc_event.h"

uwcEvent_t uwc_event_wifi_init() {
  if (uwcIsWifiInit) {
    ESP_LOGW(uwc_tag_event, "WiFI already initialized!");
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
  if (!uwcIsWifiInit) {
    ESP_LOGE(uwc_tag_event, "WiFI is not initialized!");
    return;
  }

  if (!tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &uwcIPInfo)) {
    ESP_LOGI(uwc_tag_event, "IPV4 -> " IPSTR, IP2STR(&uwcIPInfo.ip));
    ESP_LOGI(uwc_tag_event, "GTWY -> " IPSTR, IP2STR(&uwcIPInfo.gw));
    ESP_LOGI(uwc_tag_event, "NETM -> " IPSTR, IP2STR(&uwcIPInfo.netmask));
    ESP_LOGI(uwc_tag_event, "SSID -> %s", WIFI_SSID);
    ESP_LOGI(uwc_tag_event, "PASW -> %s", WIFI_PASW);
  }
}

uwcEvent_t uwc_event_wifi_setup(void) {
  // Get SSID input
  uwc_uart_input("Enter SSID: ", WIFI_SSID);
  // uwc_uart_send("Enter SSID: ");
  // for (;;) {
  //   if (uwc_uart_recv() > 0) {  // minimal SSID length is 1
  //     strcpy((char *)WIFI_SSID, uwc_uart_get_data());
  //     break;
  //   }
  // }
  // uwc_eol_remover((char *)WIFI_SSID);
  // uwc_uart_send((char *)WIFI_SSID);
  // uwc_uart_send("\n");

  // Get PASW input
  uwc_uart_input("Enter PASW: ", WIFI_PASW);
  // uwc_uart_send("Enter PASW: ");
  // for (;;) {
  //   if (uwc_uart_recv() > 0) {  // minimal password length is 8 actually
  //     strcpy((char *)WIFI_PASW, uwc_uart_get_data());
  //     break;
  //   }
  // }
  // uwc_eol_remover((char *)WIFI_PASW);
  // uwc_uart_send((char *)WIFI_PASW);
  // uwc_uart_send("\n");

  if (strlen((char *)WIFI_PASW) < 8) {
    ESP_LOGW(uwc_tag_event,
             "Warning! Password length is less than 8 characters!");
  }
  uwc_uart_send("WiFi setup has been updated!\n");
}
