#include "uwc_event.h"

static size_t readLen = sizeof(WIFI_SSID) | sizeof(WIFI_PASW);
static const char ssid_key[] = "WIFI_SSID";
static const char pasw_key[] = "WIFI_PASW";

uwcEvent_t uwc_event_nvs_init(void) {
  uwcEventErr = uwc_nvs_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_open(void) {
  uwcEventErr = uwc_nvs_open();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

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

uwcEvent_t uwc_event_nvs_commit_auth(void) {
  uwcEventErr = uwc_nvs_commit();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_nvs_close(void) {
  uwcEventErr = uwc_nvs_close();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}
