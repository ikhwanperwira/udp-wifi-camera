#include "uwc_task.h"

void app_main(void) {
  ESP_LOGI(uwc_tag, "Start of program!");

  uwc_startup_task();
  vTaskDelay(100 / portTICK_PERIOD_MS);

  xTaskCreate(uwc_uart_task, "uwc_uart_task", 4096, NULL, 5, NULL);
  vTaskDelay(100 / portTICK_PERIOD_MS);

  ESP_LOGI(uwc_tag, "End of program!");
}
