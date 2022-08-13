#include "uwc_led.h"

static esp_err_t err;

bool uwcLedIsInit = false;

esp_err_t uwc_led_init(void) {
  if (uwcLedIsInit) {
    ESP_LOGW(uwc_tag_led, "LED already initialized!");
    return ESP_OK;
  }

  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_LED);
  err = gpio_set_direction(ESP32CAM_PIN_BUILT_IN_LED, GPIO_MODE_OUTPUT);

  if (err) {
    uwcLedIsInit = false;
    ESP_LOGE(uwc_tag_led, "LED init failure!");
    return err;
  }

  ESP_LOGI(uwc_tag_led, "LED has been initialized!");
  uwcLedIsInit = true;
  return err;
}

esp_err_t uwc_led_set(bool state) {
  if (!uwcLedIsInit) {
    ESP_LOGW(uwc_tag_led, "LED is not initialized yet, initializing led...");
    uwc_led_init();
    return uwc_led_set(LED_ON);
  }

  err = gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, state);

  if (err) {
    ESP_LOGE(uwc_tag_led, "LED set failure!");
    return err;
  }

  ESP_LOGI(uwc_tag_led, "LED has been set!");
  return err;
}
