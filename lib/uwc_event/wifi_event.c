#include "uwc_event.h"

static uint8_t is_wifi_initialized = 0;

static uint8_t is_nvs_initialized = 0;

void nvs_init() {
  if (is_nvs_initialized) {
    ESP_LOGW(PROG_TAG, "NVS already initialized!\n");
    return;
  }

  // Initialize NVS
  esp_err_t nvsErr = nvs_flash_init();
  if (nvsErr == ESP_ERR_NVS_NO_FREE_PAGES ||
      nvsErr == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    nvsErr = nvs_flash_init();
  }
  ESP_ERROR_CHECK(nvsErr);

  if (nvsErr == ESP_OK) {
    ESP_LOGI(PROG_TAG, "NVS has been initialized!\n");
    is_nvs_initialized = 1;
    return;
  }

  ESP_LOGE(PROG_TAG, "NVS init failure!\n");
}

void wifi_init_sta() {
  if (is_wifi_initialized) {
    ESP_LOGW(PROG_TAG, "WiFi already initialized!\n");
    return;
  }

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
              .ssid = WIFI_SSID,
              .password = WIFI_PASW,
              /* Setting a password implies station will connect to all security
               * modes including WEP/WPA. However these modes are deprecated and
               * not advisable to be used. Incase your Access point doesn't
               * support WPA2, these mode can be enabled by commenting below
               * line */
              .threshold.authmode = WIFI_AUTH,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(PROG_TAG, "wifi_init_sta finished.");

  /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
   * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
   * bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(PROG_TAG, "Connected to ap SSID:%s password:%s", WIFI_SSID,
             WIFI_PASW);
    ESP_LOGI(PROG_TAG, "WiFi has been initialized!\n");
    is_wifi_initialized = 1;
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(PROG_TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID,
             WIFI_PASW);
  } else {
    ESP_LOGE(PROG_TAG, "UNEXPECTED EVENT");
  }
}

void wifi_deinit_sta() {
  if (!is_wifi_initialized) {
    ESP_LOGE(PROG_TAG, "WiFi already deinitialized!\n");
    return;
  }

  if (esp_wifi_deinit() == ESP_OK) {
    ESP_LOGI(PROG_TAG, "WiFi has been deinitialized!\n");
    is_wifi_initialized = 0;
    return;
  }

  ESP_LOGE(PROG_TAG, "WiFi init failure!\n");
}

void wifi_ip_sta_show() {
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
  ESP_LOGI(PROG_TAG, "%s", tmp);
}