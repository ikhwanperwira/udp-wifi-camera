#include "uwc_nvs.h"

static esp_err_t err;

bool uwcNvsIsInit = false;
bool uwcNvsIsOpen = false;

esp_err_t uwc_nvs_init(void) {
  if (uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS already initialized!");
    return ESP_OK;
  }

  // Initialize NVS
  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  if (!err) {
    ESP_LOGI(uwc_tag_nvs, "NVS has been initialized!");
    uwcNvsIsInit = true;
    return err;
  }

  ESP_LOGE(uwc_tag_nvs, "NVS init failure! %s", esp_err_to_name(err));
  return err;
}

esp_err_t uwc_nvs_open(void) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS already opened!");
    return ESP_OK;
  }

  // Open NVS
  err = nvs_open("uwc", NVS_READWRITE, &uwcNvsHandler);
  if (!err) {
    ESP_LOGI(uwc_tag_nvs, "NVS has been opened!");
    uwcNvsIsOpen = true;
    return err;
  }

  ESP_LOGE(uwc_tag_nvs, "NVS open failure! %s", esp_err_to_name(err));
  return err;
}

esp_err_t uwc_nvs_read(const char* key, char* val_out, size_t* len) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not opened yet!");
    return ESP_OK;
  }

  // Read string value by key.
  err = nvs_get_str(uwcNvsHandler, key, val_out, len);
  switch (err) {
    case ESP_OK:
      ESP_LOGI(uwc_tag_nvs, "read %s: %s (%i)", key, val_out, strlen(val_out));
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      ESP_LOGW(uwc_tag_nvs, "%s key is not initialized yet!", key);
      break;
    default:
      ESP_LOGE(uwc_tag_nvs, "%s read failure! %s", key, esp_err_to_name(err));
  }
  return err;
}

esp_err_t uwc_nvs_write(const char* key, char* val_in) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not opened yet!");
    return ESP_OK;
  }

  // Write string value by key.
  err = nvs_set_str(uwcNvsHandler, key, val_in);
  if (!err) {
    ESP_LOGI(uwc_tag_nvs, "write %s: %s (%i)", key, val_in, strlen(val_in));
    return err;
  }
  ESP_LOGE(uwc_tag_nvs, "%s write %s failure! %s", key, val_in,
           esp_err_to_name(err));
  return err;
}

esp_err_t uwc_nvs_erase(const char* key) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not opened yet!");
    return ESP_OK;
  }

  // Erase key.
  err = nvs_erase_key(uwcNvsHandler, key);
  if (!err) {
    ESP_LOGI(uwc_tag_nvs, "%s key has been erased!", key);
    return err;
  }
  ESP_LOGE(uwc_tag_nvs, "%s erase failure! %s", key, esp_err_to_name(err));
  return err;
}

esp_err_t uwc_nvs_commit(void) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not opened yet!");
    return ESP_OK;
  }

  // Commit NVS
  err = nvs_commit(uwcNvsHandler);
  if (!err) {
    ESP_LOGI(uwc_tag_nvs, "NVS has been commited!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not opened yet!");
    return ESP_OK;
  }

  ESP_LOGE(uwc_tag_nvs, "NVS commit failure! %s", esp_err_to_name(err));
  return err;
}

esp_err_t uwc_nvs_close(void) {
  if (!uwcNvsIsInit) {
    ESP_LOGW(uwc_tag_nvs, "NVS is not initialized yet!");
    return ESP_OK;
  }

  if (!uwcNvsIsOpen) {
    ESP_LOGW(uwc_tag_nvs, "NVS already closed!");
    return ESP_OK;
  }

  // Close NVS
  nvs_close(uwcNvsHandler);
  ESP_LOGI(uwc_tag_nvs, "NVS has been closed!");
  uwcNvsIsOpen = false;
  return err;
};
