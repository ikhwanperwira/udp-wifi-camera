#include "uwc_task.h"

uwcTask_t uwc_task_startup() {
  uwc_event_nvs_init();
  uwc_event_nvs_open();
  uwc_event_nvs_read_ssid();
  uwc_event_nvs_read_pasw();
  ESP_LOGI(uwc_tag_task, "WiFi setup: (%s|%s)", WIFI_SSID, WIFI_PASW);
  ESP_LOGI(uwc_tag_task, "Startup finished!");
}
