#include "uwc_event.h"

void led_init() {
  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_LED);

  if (gpio_set_direction(ESP32CAM_PIN_BUILT_IN_LED, GPIO_MODE_OUTPUT) ==
      ESP_OK) {
    ESP_LOGI(PROG_TAG, "LED has been initialized!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED init failure!\n");
}

void led_set_on() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 0) == ESP_OK) {
    ESP_LOGI(PROG_TAG, "LED has been turned on!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED set on failure!\n");
}

void led_set_off() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 1) == ESP_OK) {
    ESP_LOGI(PROG_TAG, "LED has been turned off!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED set off failure!\n");
}