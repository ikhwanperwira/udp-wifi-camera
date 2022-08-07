#include "uwc_cam.h"

static esp_err_t err;

// Default camera config OV2640.
static const camera_config_t uwcCamCfg = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,
    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 10000000,  // EXPERIMENTAL: Set to 16MHz on ESP32-S2 or
                               // ESP32-S3 to enable EDMA mode
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,  // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size =
        FRAMESIZE_HVGA,  // QQVGA-QXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 8,  // 0-63 lower number means higher quality
    .fb_count =
        2,  // if more than one, i2s runs in continuous mode. Use only with JPEG
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY  // CAMERA_GRAB_LATEST. Sets when
                                         // buffers should be filled
};

bool uwcCamIsInit = false;
camera_fb_t* uwcCamFb = NULL;

esp_err_t uwc_cam_init(void) {
  if (uwcCamIsInit) {
    ESP_LOGW(uwc_tag_cam, "Camera already initialized!");
    return ESP_OK;
  }

  err = esp_camera_init(&uwcCamCfg);

  if (!err) {
    ESP_LOGI(uwc_tag_cam, "Camera has been initialized!");
    uwcCamIsInit = true;
    return err;
  }

  ESP_LOGE(uwc_tag_cam, "Camera init failure!");
  return err;
}

esp_err_t uwc_cam_deinit(void) {
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_cam, "Camera already deinitialized!");
    return ESP_OK;
  }

  err = esp_camera_deinit();

  if (!err) {
    ESP_LOGI(uwc_tag_cam, "Camera has been deinitialized!");
    uwcCamIsInit = false;
    return err;
  }

  ESP_LOGE(uwc_tag_cam, "Camera deinit failure!");
  return err;
}

esp_err_t uwc_cam_open(void) {
  uwcCamFb = esp_camera_fb_get();
  if (!uwcCamFb) {
    ESP_LOGW(uwc_tag_cam, "Camera capture failed!");
    return ESP_FAIL;
  }

  return ESP_OK;
}

void uwc_cam_close(void) { esp_camera_fb_return(uwcCamFb); }
