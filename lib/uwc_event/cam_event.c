#include "uwc_event.h"

static bool isCamInit = false;

void uwc_cam_init() {
  if (isCamInit) {
    ESP_LOGW(UWC_TAG, "Camera already initialized!");
    return;
  }

  if (esp_camera_init(&uwc_cam_cfg) == ESP_OK) {
    ESP_LOGI(UWC_TAG, "Camera has been initialized!");
    isCamInit = true;
    return;
  }

  ESP_LOGE(UWC_TAG, "Camera init failure!");
}

void uwc_cam_deinit() {
  if (!isCamInit) {
    ESP_LOGW(UWC_TAG, "Camera already deinitialized!");
    return;
  }

  if (esp_camera_deinit() == ESP_OK) {
    ESP_LOGI(UWC_TAG, "Camera has been deinitialized!");
    isCamInit = false;
    return;
  }

  ESP_LOGE(UWC_TAG, "Camera deinit failure!");
}