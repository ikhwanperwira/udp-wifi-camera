#include "uwc_event.h"

static bool isWifiInit = 0;
void uwc_wifi_init_sta() {
  if (isWifiInit) {
    ESP_LOGW(UWC_TAG, "WiFi already initialized!");
    return;
  }

  uwc_nvs_init();

  s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

  wifi_config_t wifi_config = {
      .sta =
          {
              .threshold.authmode = WIFI_AUTH,
          },
  };

  strncpy((char *)wifi_config.sta.ssid, (char *)&WIFI_SSID[0], 32);
  strncpy((char *)wifi_config.sta.password, (char *)&WIFI_PASW[0], 64);

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(UWC_TAG, "wifi_init_sta finished!");

  /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
   * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
   * bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(UWC_TAG, "Connected to ap SSID:%s password:%s", WIFI_SSID,
             WIFI_PASW);
    ESP_LOGI(UWC_TAG, "WiFi has been initialized!");
    isWifiInit = 1;
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(UWC_TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID,
             WIFI_PASW);
  } else {
    ESP_LOGE(UWC_TAG, "UNEXPECTED EVENT");
  }
}

void uwc_wifi_deinit_sta() {
  if (!isWifiInit) {
    ESP_LOGE(UWC_TAG, "WiFi already deinitialized!");
    return;
  }

  if (esp_wifi_deinit() == ESP_OK) {
    ESP_LOGI(UWC_TAG, "WiFi has been deinitialized!");
    isWifiInit = 0;
    return;
  }

  ESP_LOGE(UWC_TAG, "WiFi init failure!");
}

void uwc_wifi_ip() {
  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
  char ip[32];
  char gw[32];
  char nm[32];
  char tmp[64] = "\0";
  sprintf(ip, "IP: " IPSTR "\n", IP2STR(&ipInfo.ip));
  sprintf(gw, "GW: " IPSTR "\n", IP2STR(&ipInfo.gw));
  sprintf(nm, "NM: " IPSTR "\n", IP2STR(&ipInfo.netmask));
  strcat(tmp, ip);
  strcat(tmp, gw);
  strcat(tmp, nm);
  ESP_LOGI(UWC_TAG, "\n%s", tmp);
}

void uwc_wifi_set() {
  uwc_uart_send("Enter SSID: ");
  for (;;) {
    if (uwc_uart_recv() > 0) {  // minimal SSID length is 1
      strcpy((char *)WIFI_SSID, uwc_uart_get_data());
      break;
    }
  }
  uwc_eol_remover((char *)WIFI_SSID);
  uwc_uart_send((char *)WIFI_SSID);
  uwc_uart_send("\n");

  uwc_uart_send("Enter PASW: ");
  for (;;) {
    if (uwc_uart_recv() > 0) {  // minimal password length is 8 actually
      strcpy((char *)WIFI_PASW, uwc_uart_get_data());
      break;
    }
  }
  uwc_eol_remover((char *)WIFI_PASW);
  uwc_uart_send((char *)WIFI_PASW);
  uwc_uart_send("\n");

  if (strlen((char *)WIFI_PASW) < 8) {
    ESP_LOGW(UWC_TAG, "Warning! Password length is less than 8 characters!");
  }
  uwc_uart_send("WiFi setup has been updated!\n");
}