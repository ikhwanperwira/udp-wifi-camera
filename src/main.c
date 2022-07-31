#include "uwc_event.h"

void app_main() {
  ESP_LOGI(PROG_TAG, "Start of program!");

  if (uart_cmd_init(115200) == ESP_OK) {
    uart_cmd_send("\nUART CMD has been initialized!\n");
  } else {
    ESP_LOGE(PROG_TAG, "UART CMD init failure!");
  }

  uart_cmd_send("WiFi setup: (" WIFI_SSID "|" WIFI_PASW ")\n");

  for (;;) {
    uart_cmd_recv();
    uart_cmd_on("$nvs init\n", nvs_init, NULL);
    uart_cmd_on("$wifi init\n", wifi_init_sta, NULL);
    uart_cmd_on("$led init\n", led_init, NULL);
    uart_cmd_on("$led on\n", led_set_on, NULL);
    uart_cmd_on("$led off\n", led_set_off, NULL);
    uart_cmd_on("$cam init\n", camera_init, NULL);
    uart_cmd_on("$cam deinit\n", camera_deinit, NULL);
    uart_cmd_on("$wifi init\n", wifi_init_sta, NULL);
    uart_cmd_on("$wifi deinit\n", wifi_deinit_sta, NULL);
    uart_cmd_on("$ip show\n", wifi_ip_sta_show, NULL);

    if (uart_cmd_is_data_match("$exit\n")) {
      uart_cmd_send("Exiting program...\n");
      break;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(PROG_TAG, "End of program!");
  return;
}