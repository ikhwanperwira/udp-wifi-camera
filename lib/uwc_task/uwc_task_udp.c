#include "uwc_task.h"

uwcTask_t uwc_task_udp(void *pvParameters) {
  uwc_event_udp_init();
  for (;;) {
    uwc_udp_recv();

    // NVS handler.
    uwc_udp_on("$nvs init\n", uwc_event_nvs_init, NULL);
    uwc_udp_on("$nvs open\n", uwc_event_nvs_open, NULL);
    uwc_udp_on("$nvs read ssid\n", uwc_event_nvs_read_ssid, NULL);
    uwc_udp_on("$nvs write ssid\n", uwc_event_nvs_write_ssid, NULL);
    uwc_udp_on("$nvs erase ssid\n", uwc_event_nvs_erase_ssid, NULL);
    uwc_udp_on("$nvs read pasw\n", uwc_event_nvs_read_pasw, NULL);
    uwc_udp_on("$nvs write pasw\n", uwc_event_nvs_write_pasw, NULL);
    uwc_udp_on("$nvs erase pasw\n", uwc_event_nvs_erase_pasw, NULL);
    uwc_udp_on("$nvs commit\n", uwc_event_nvs_commit_auth, NULL);
    uwc_udp_on("$nvs close\n", uwc_event_nvs_close, NULL);

    // LED handler.
    uwc_udp_on("$led init\n", uwc_event_led_init, NULL);
    uwc_udp_on("$led on\n", uwc_event_led_set_on, NULL);
    uwc_udp_on("$led off\n", uwc_event_led_set_off, NULL);

    // Cam handler.
    uwc_udp_on("$cam init\n", uwc_event_cam_init, NULL);
    uwc_udp_on("$cam deinit\n", uwc_event_cam_deinit, NULL);

    // WiFi handler.
    uwc_udp_on("$wifi init\n", uwc_event_wifi_init, NULL);
    uwc_udp_on("$wifi deinit\n", uwc_event_wifi_deinit, NULL);
    uwc_udp_on("$wifi setup\n", uwc_event_wifi_setup, NULL);
    uwc_udp_on("$wifi info\n", uwc_event_wifi_info, NULL);

    if (uwc_udp_is_data_match("$exit\n")) {
      break;
    } else if (uwc_udp_is_data_match("$restart\n")) {
      uwc_udp_send("Restarting program...\n");
      esp_restart();
    }
  }
  ESP_LOGW(uwc_tag_task, "Exited!");
  vTaskDelete(NULL);
}
