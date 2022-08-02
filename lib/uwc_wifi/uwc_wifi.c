#include "uwc_wifi.h"

int s_retry_num = 0;

char WIFI_SSID[32] = "uwc";
char WIFI_PASW[32] = "87654321";

bool uwcIsWifiInit = false;
static esp_err_t err;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    uwcIsWifiInit = false;
    if (s_retry_num < MAX_TRY) {
      esp_wifi_connect();
      s_retry_num++;
      ESP_LOGI(uwc_tag_wifi, "retry to connect to the AP");
    } else {
      xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }
    ESP_LOGI(uwc_tag_wifi, "connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(uwc_tag_wifi, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

esp_err_t uwc_wifi_init_sta(void) {
  if (uwcIsWifiInit) {
    ESP_LOGW(uwc_tag_wifi, "WiFi already initialized!");
    return ESP_OK;
  }

  s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  err = esp_wifi_init(&cfg);
  ESP_ERROR_CHECK(err);

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

  ESP_LOGI(uwc_tag_wifi, "wifi_init_sta finished!");

  /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
   * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
   * bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(uwc_tag_wifi, "Connected to ap SSID:%s password:%s", WIFI_SSID,
             WIFI_PASW);
    ESP_LOGI(uwc_tag_wifi, "WiFi has been initialized!");
    uwcIsWifiInit = true;
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(uwc_tag_wifi, "Failed to connect to SSID:%s, password:%s",
             WIFI_SSID, WIFI_PASW);
    uwcIsWifiInit = false;
  } else {
    ESP_LOGE(uwc_tag_wifi, "UNEXPECTED EVENT");
    uwcIsWifiInit = false;
  }

  return err;
}

esp_err_t uwc_wifi_deinit_sta(void) {
  if (!uwcIsWifiInit) {
    ESP_LOGE(uwc_tag_wifi, "WiFi already deinitialized!");
    return ESP_OK;
  }

  err = esp_wifi_deinit();

  if (!err) {
    ESP_LOGI(uwc_tag_wifi, "WiFi has been deinitialized!");
    uwcIsWifiInit = false;
    return err;
  }

  ESP_LOGE(uwc_tag_wifi, "WiFi init failure!");
  return err;
}
