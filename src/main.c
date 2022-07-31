#include "uwc_event.h"

void app_main() {
  ESP_LOGI(UWC_TAG, "Start of program!");

  esp_err_t err = uwc_uart_init(115200);
  if (err == ESP_OK) {
    uwc_uart_send("\nUART CMD has been initialized!\n");
  } else {
    ESP_LOGE(UWC_TAG, "UART CMD init failure!");
  }

  // NVS Initialization
  uwc_nvs_init();
  uwc_nvs_open();
  uwc_nvs_read_ssid();
  uwc_nvs_read_pasw();

  // WIFi setup: (ssid|pasw)
  uwc_uart_send("WiFi setup: (");
  uwc_uart_send((char*)WIFI_SSID);
  uwc_uart_send("|");
  uwc_uart_send((char*)WIFI_PASW);
  uwc_uart_send(")\n");

  for (;;) {
    uwc_uart_recv();  // Storing data received into buffer.

    // NVS handler.
    uwc_uart_on("$nvs init\n", uwc_nvs_init, NULL);
    uwc_uart_on("$nvs open\n", uwc_nvs_open, NULL);
    uwc_uart_on("$nvs read ssid\n", uwc_nvs_read_ssid, NULL);
    uwc_uart_on("$nvs write ssid\n", uwc_nvs_write_ssid, NULL);
    uwc_uart_on("$nvs erase ssid\n", uwc_nvs_erase_ssid, NULL);
    uwc_uart_on("$nvs read pasw\n", uwc_nvs_read_pasw, NULL);
    uwc_uart_on("$nvs write pasw\n", uwc_nvs_write_pasw, NULL);
    uwc_uart_on("$nvs erase pasw\n", uwc_nvs_erase_pasw, NULL);
    uwc_uart_on("$nvs commit\n", uwc_nvs_commit, NULL);
    uwc_uart_on("$nvs close\n", uwc_nvs_close, NULL);

    // LED handler.
    uwc_uart_on("$led init\n", uwc_led_init, NULL);
    uwc_uart_on("$led on\n", uwc_led_set_on, NULL);
    uwc_uart_on("$led off\n", uwc_led_set_off, NULL);

    // Cam handler.
    uwc_uart_on("$cam init\n", uwc_cam_init, NULL);
    uwc_uart_on("$cam deinit\n", uwc_cam_deinit, NULL);

    // Wifi handler.
    uwc_uart_on("$wifi init\n", uwc_wifi_init_sta, NULL);
    uwc_uart_on("$wifi deinit\n", uwc_wifi_deinit_sta, NULL);
    uwc_uart_on("$wifi set\n", uwc_wifi_set, NULL);
    uwc_uart_on("$wifi ip\n", uwc_wifi_ip, NULL);

    if (uwc_uart_is_data_match("$exit\n")) {
      uwc_uart_send("Exiting program...\n");
      break;
    } else if (uwc_uart_is_data_match("$restart\n")) {
      uwc_uart_send("Restarting program...\n");
      esp_restart();
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(UWC_TAG, "End of program, system halt!");
  return;
}