#include "uwc_task.h"

void app_main(void) {
  ESP_LOGI(uwc_tag, "Start of app_main()!");

  uwc_task_startup();
  vTaskDelay(100 / portTICK_PERIOD_MS);

  xTaskCreate(uwc_task_uart, "uwc_task_uart", 4096, NULL, 6, NULL);
  vTaskDelay(100 / portTICK_PERIOD_MS);

  uwc_event_udp_init();

  ESP_LOGI(uwc_tag, "End of app_main()");
}
