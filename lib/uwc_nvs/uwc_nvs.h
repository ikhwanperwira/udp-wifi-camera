/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_nvs.
 *
 */

#ifndef __UWC_NVS_H__
#define __UWC_NVS_H__

// Dependencies
#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include "uwc_tag.h"

// NVS initialization status.
extern bool uwcIsNvsInit;
extern bool uwcIsNvsOpen;

/**
 * @brief Handler for NVS in "uwc" storage.
 */
nvs_handle_t uwcNvsHandler;

/**
 * @brief call nvs_flash_init.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_init(void);

/**
 * @brief Open NVS with using namespace "uwc".
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_open(void);

/**
 * @brief Read string value in "uwc" by given key.
 * @param key: a key.
 * @param val_out: value output.
 * @param len: length of value output, higher len for safety from error but
 * require more memory.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_read(const char* key, char* val_out, size_t* len);

/**
 * @brief Write string value in "uwc" by given key.
 * @param key: a key.
 * @param val_in: value input.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_write(const char* key, char* val_in);

/**
 * @brief Erase ke in "uwc".
 * @param key: a key.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_erase(const char* key);

/**
 * @brief Commit any changes (do flashing). Decreasing lifetime of NVS.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_commit(void);

/**
 * @brief Close NVS if not needed for free memory.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_nvs_close(void);

#endif
