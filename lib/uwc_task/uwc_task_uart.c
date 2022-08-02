#include "uwc_task.h"

uwcTask_t uwc_task_uart(void *pvParameters) {
  esp_err_t err = uwc_uart_init(115200);
  if (!err) {
    uwc_uart_send("\nUART CMD has been initialized!\n");
  } else {
    ESP_LOGE(uwc_tag_task, "UART CMD init failure!");
  }
  for (;;) {
    uwc_uart_recv();  // Storing data received into buffer.

    // NVS handler.
    uwc_uart_on("$nvs init\n", uwc_event_nvs_init, NULL);
    uwc_uart_on("$nvs open\n", uwc_event_nvs_open, NULL);
    uwc_uart_on("$nvs read ssid\n", uwc_event_nvs_read_ssid, NULL);
    uwc_uart_on("$nvs write ssid\n", uwc_event_nvs_write_ssid, NULL);
    uwc_uart_on("$nvs erase ssid\n", uwc_event_nvs_erase_ssid, NULL);
    uwc_uart_on("$nvs read pasw\n", uwc_event_nvs_read_pasw, NULL);
    uwc_uart_on("$nvs write pasw\n", uwc_event_nvs_write_pasw, NULL);
    uwc_uart_on("$nvs erase pasw\n", uwc_event_nvs_erase_pasw, NULL);
    uwc_uart_on("$nvs commit\n", uwc_event_nvs_commit_auth, NULL);
    uwc_uart_on("$nvs close\n", uwc_event_nvs_close, NULL);

    // LED handler.
    uwc_uart_on("$led init\n", uwc_event_led_init, NULL);
    uwc_uart_on("$led on\n", uwc_event_led_set_on, NULL);
    uwc_uart_on("$led off\n", uwc_event_led_set_off, NULL);

    // Cam handler.
    uwc_uart_on("$cam init\n", uwc_event_cam_init, NULL);
    uwc_uart_on("$cam deinit\n", uwc_event_cam_deinit, NULL);

    // WiFi handler.
    uwc_uart_on("$wifi init\n", uwc_event_wifi_init, NULL);
    uwc_uart_on("$wifi deinit\n", uwc_event_wifi_deinit, NULL);
    uwc_uart_on("$wifi setup\n", uwc_event_wifi_setup, NULL);
    uwc_uart_on("$wifi info\n", uwc_event_wifi_info, NULL);

    // UDP handler.
    uwc_uart_on("$udp init\n", uwc_event_udp_init, NULL);
    uwc_uart_on("$udp debug\n", uwc_udp_debug, NULL);

    if (uwc_uart_is_data_match("$exit\n")) {
      break;
    } else if (uwc_uart_is_data_match("$restart\n")) {
      uwc_uart_send("Restarting program...\n");
      esp_restart();
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  ESP_LOGW(uwc_tag_task, "Exited!");
  vTaskDelete(NULL);
}
