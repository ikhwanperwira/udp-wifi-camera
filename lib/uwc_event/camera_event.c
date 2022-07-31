#include "uwc_event.h"

static uint8_t is_camera_initialized = 0;

void camera_init() {
  if (is_camera_initialized) {
    ESP_LOGW(PROG_TAG, "Camera already initialized!\n");
    return;
  }

  if (esp_camera_init(&cam_cfg) == ESP_OK) {
    ESP_LOGI(PROG_TAG, "Camera has been initialized!\n");
    is_camera_initialized = 1;
    return;
  }

  ESP_LOGE(PROG_TAG, "Camera init failure!\n");
}

void camera_deinit() {
  if (!is_camera_initialized) {
    ESP_LOGW(PROG_TAG, "Camera already deinitialized!\n");
    return;
  }

  if (esp_camera_deinit() == ESP_OK) {
    ESP_LOGI(PROG_TAG, "Camera has been deinitialized!\n");
    is_camera_initialized = 0;
    return;
  }

  ESP_LOGE(PROG_TAG, "Camera deinit failure!\n");
}