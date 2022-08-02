#include "uwc_task.h"

void app_main(void) {
  ESP_LOGI(uwc_tag, "Start of program!");

  uwc_task_startup();
  vTaskDelay(100 / portTICK_PERIOD_MS);

  // xTaskCreate(uwc_task_uart, "uwc_task_uart", 4096, NULL, 6, NULL);
  // vTaskDelay(100 / portTICK_PERIOD_MS);

  xTaskCreate(uwc_task_udp, "uwc_task_udp", 4096, NULL, 5, NULL);
  vTaskDelay(100 / portTICK_PERIOD_MS);

  ESP_LOGI(uwc_tag, "End of program!");
}
