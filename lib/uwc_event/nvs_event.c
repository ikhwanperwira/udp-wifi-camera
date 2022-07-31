#include "uwc_event.h"
static esp_err_t err;

nvs_handle_t uwcNVSHandler;
static bool isNVSInit = 0;
static bool isNVSOpen = 0;
static size_t len = 32;  // max len WIFI_SSID and WIFI_PASW

void uwc_nvs_init() {
  if (isNVSInit) {
    ESP_LOGW(UWC_TAG, "NVS already initialized!");
    return;
  }

  // Initialize NVS
  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS has been initialized!");
    isNVSInit = true;
    return;
  }

  ESP_LOGE(UWC_TAG, "NVS init failure! %s", esp_err_to_name(err));
}

void uwc_nvs_open() {
  if (isNVSOpen) {
    ESP_LOGW(UWC_TAG, "NVS already opened!");
    return;
  }

  // Open NVS
  err = nvs_open("uwc", NVS_READWRITE, &uwcNVSHandler);
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS has been opened!");
    isNVSOpen = true;
    return;
  }

  ESP_LOGE(UWC_TAG, "NVS open failure! %s", esp_err_to_name(err));
}

void uwc_nvs_read_ssid() {
  // Read SSID from "uwc"
  len = 32;
  err = nvs_get_str(uwcNVSHandler, "WIFI_SSID", (char *)WIFI_SSID, &len);
  switch (err) {
    case ESP_OK:
      ESP_LOGI(UWC_TAG, "NVS WIFI_SSID read: %s (%i)", WIFI_SSID,
               strlen((const char *)WIFI_SSID));
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      ESP_LOGW(UWC_TAG, "NVS WIFI_SSID key is not initialized yet!");
      break;
    default:
      ESP_LOGE(UWC_TAG, "NVS WIFI_SSID read failure! %s", esp_err_to_name(err));
  }
}

void uwc_nvs_write_ssid() {
  // Write SSID to "uwc"
  err = nvs_set_str(uwcNVSHandler, "WIFI_SSID", (char *)WIFI_SSID);
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS WIFI_SSID write: %s", WIFI_SSID);
    return;
  }
  ESP_LOGE(UWC_TAG, "NVS WIFI_SSID write failure! %s", esp_err_to_name(err));
}

void uwc_nvs_erase_ssid() {
  // Erase SSID key in "uwc"
  err = nvs_erase_key(uwcNVSHandler, "WIFI_SSID");
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS WIFI_SSID key has been erased!");
    return;
  }
  ESP_LOGE(UWC_TAG, "NVS WIFI_SSID key erase failure! %s",
           esp_err_to_name(err));
}

void uwc_nvs_read_pasw() {
  // Read PASW from "uwc"
  len = 32;
  err = nvs_get_str(uwcNVSHandler, "WIFI_PASW", (char *)WIFI_PASW, &len);
  switch (err) {
    case ESP_OK:
      ESP_LOGI(UWC_TAG, "NVS WIFI_PASW read: %s (%i)", WIFI_PASW,
               strlen((const char *)WIFI_PASW));
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      ESP_LOGW(UWC_TAG, "NVS WIFI_PASW key is not initialized yet!");
      break;
    default:
      ESP_LOGE(UWC_TAG, "NVS WIFI_PASW read failure! %s", esp_err_to_name(err));
  }
}

void uwc_nvs_write_pasw() {
  // Write PASW to "uwc"
  err = nvs_set_str(uwcNVSHandler, "WIFI_PASW", (char *)WIFI_PASW);
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS WIFI_PASW write: %s", WIFI_PASW);
    return;
  }
  ESP_LOGE(UWC_TAG, "NVS WIFI_PASW write failure! %s", esp_err_to_name(err));
}

void uwc_nvs_erase_pasw() {
  // Erase PASW key in "uwc"
  err = nvs_erase_key(uwcNVSHandler, "WIFI_PASW");
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS WIFI_PASW key has been erased!");
    return;
  }
  ESP_LOGE(UWC_TAG, "NVS WIFI_PASW key erase failure! %s",
           esp_err_to_name(err));
}

void uwc_nvs_commit() {
  // Commit NVS
  err = nvs_commit(uwcNVSHandler);
  if (err == ESP_OK) {
    ESP_LOGI(UWC_TAG, "NVS has been commited!");
    return;
  }
  ESP_LOGE(UWC_TAG, "NVS commit failure! %s", esp_err_to_name(err));
}

void uwc_nvs_close() {
  // Close NVS
  nvs_close(uwcNVSHandler);
};