#include "uwc_event.h"

static size_t readLen = 32;
static const char ssid_key[] = "WIFI_SSID";
static const char pasw_key[] = "WIFI_PASW";
static const char framesize_key[] = "CAM_FRAMESIZE";
static const char jpeglevel_key[] = "CAM_JPEGLEVEL";

uwcEvent_t uwc_event_nvs_init(void) {
  uwcEventErr = uwc_nvs_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_open(void) {
  uwcEventErr = uwc_nvs_open();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

// WIFI_SSID
uwcEvent_t uwc_event_nvs_read_ssid(void) {
  readLen = sizeof(WIFI_SSID);
  uwcEventErr = uwc_nvs_read(ssid_key, WIFI_SSID, &readLen);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_write_ssid(void) {
  uwcEventErr = uwc_nvs_write(ssid_key, WIFI_SSID);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_erase_ssid(void) {
  uwcEventErr = uwc_nvs_erase(ssid_key);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

// WIFI_PASW
uwcEvent_t uwc_event_nvs_read_pasw(void) {
  readLen = sizeof(WIFI_PASW);
  uwcEventErr = uwc_nvs_read(pasw_key, WIFI_PASW, &readLen);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_write_pasw(void) {
  uwcEventErr = uwc_nvs_write(pasw_key, WIFI_PASW);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_erase_pasw(void) {
  uwcEventErr = uwc_nvs_erase(pasw_key);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

// FRAME_SIZE
uwcEvent_t uwc_event_nvs_read_framesize(void) {
  readLen = sizeof(uwcCamFrameSize);
  uwcEventErr = uwc_nvs_read(framesize_key, uwcCamFrameSize, &readLen);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_write_framesize(void) {
  uwcEventErr = uwc_nvs_write(framesize_key, uwcCamFrameSize);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_erase_framesize(void) {
  uwcEventErr = uwc_nvs_erase(framesize_key);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

// JPEG_LEVEL
uwcEvent_t uwc_event_nvs_read_jpeglevel(void) {
  readLen = sizeof(uwcCamJpegCompression);
  uwcEventErr = uwc_nvs_read(jpeglevel_key, uwcCamJpegCompression, &readLen);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_write_jpeglevel(void) {
  uwcEventErr = uwc_nvs_write(jpeglevel_key, uwcCamJpegCompression);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_erase_jpeglevel(void) {
  uwcEventErr = uwc_nvs_erase(jpeglevel_key);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_commit_auth(void) {
  uwcEventErr = uwc_nvs_commit();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_close(void) {
  uwcEventErr = uwc_nvs_close();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}
