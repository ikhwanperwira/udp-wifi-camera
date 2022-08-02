#include "uwc_cam.h"

static esp_err_t err;

bool uwcIsCamInit = false;

esp_err_t uwc_cam_init(void) {
  if (uwcIsCamInit) {
    ESP_LOGW(uwc_tag_cam, "Camera already initialized!");
    return ESP_OK;
  }

  err = esp_camera_init(&uwc_cam_cfg);

  if (!err) {
    ESP_LOGI(uwc_tag_cam, "Camera has been initialized!");
    uwcIsCamInit = true;
    return err;
  }

  ESP_LOGE(uwc_tag_cam, "Camera init failure!");
  return err;
}

esp_err_t uwc_cam_deinit(void) {
  if (!uwcIsCamInit) {
    ESP_LOGW(uwc_tag_cam, "Camera already deinitialized!");
    return ESP_OK;
  }

  err = esp_camera_deinit();

  if (!err) {
    ESP_LOGI(uwc_tag_cam, "Camera has been deinitialized!");
    uwcIsCamInit = false;
    return err;
  }

  ESP_LOGE(uwc_tag_cam, "Camera deinit failure!");
  return err;
}
