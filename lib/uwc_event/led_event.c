#include "uwc_event.h"

void uwc_led_init() {
  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_LED);

  if (gpio_set_direction(ESP32CAM_PIN_BUILT_IN_LED, GPIO_MODE_OUTPUT) ==
      ESP_OK) {
    ESP_LOGI(UWC_TAG, "LED has been initialized!");
    return;
  }

  ESP_LOGE(UWC_TAG, "LED init failure!");
}

void uwc_led_set_on() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 0) == ESP_OK) {
    ESP_LOGI(UWC_TAG, "LED has been turned on!");
    return;
  }

  ESP_LOGE(UWC_TAG, "LED set on failure!");
}

void uwc_led_set_off() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 1) == ESP_OK) {
    ESP_LOGI(UWC_TAG, "LED has been turned off!");
    return;
  }

  ESP_LOGE(UWC_TAG, "LED set off failure!");
}