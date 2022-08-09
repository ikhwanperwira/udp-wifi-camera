#include "uwc_flashlight.h"

static esp_err_t err;

bool uwcFlashlightIsInit = false;

esp_err_t uwc_flashlight_init(void) {
  if (uwcFlashlightIsInit) {
    ESP_LOGW(uwc_tag_flashlight, "Flashlight already initialized!");
    return ESP_OK;
  }

  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_FLASHLIGHT);
  err = gpio_set_direction(ESP32CAM_PIN_BUILT_IN_FLASHLIGHT, GPIO_MODE_OUTPUT);

  if (!err) {
    ESP_LOGI(uwc_tag_flashlight, "Flashlight has been initialized!");
    uwcFlashlightIsInit = true;
    return err;
  }

  uwcFlashlightIsInit = false;
  ESP_LOGE(uwc_tag_flashlight, "Flashlight init failure!");
  return err;
}

esp_err_t uwc_flashlight_set(bool state) {
  if (!uwcFlashlightIsInit) {
    ESP_LOGW(uwc_tag_flashlight,
             "Flashlight is not initialized yet, initializing flashlight...");
    uwc_flashlight_init();
    return uwc_flashlight_set(FLASHLIGHT_ON);
  }

  err = gpio_set_level(ESP32CAM_PIN_BUILT_IN_FLASHLIGHT, state);

  if (!err) {
    ESP_LOGI(uwc_tag_flashlight, "Flashlight has been set!");
    return err;
  }

  ESP_LOGE(uwc_tag_flashlight, "Flashlight has been set!");
  return err;
}
