/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_flashlight.
 *
 */

#ifndef __UWC_FLASHLIGHT_H__
#define __UWC_FLASHLIGHT_H__

// Predefined
#define ESP32CAM_PIN_BUILT_IN_FLASHLIGHT GPIO_NUM_4
#define FLASHLIGHT_ON 1
#define FLASHLIGHT_OFF 0

// Dependencies
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "stdbool.h"
#include "uwc_tag.h"

/**
 * @brief FLASHLIGHT initialization status.
 */
extern bool uwcFlashlightIsInit;

/**
 * @brief Initializing built-in FLASHLIGHT ESP32 Camera AI-Thinker mode output.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_flashlight_init(void);

/**
 * @brief Set FLASHLIGHT state.
 * @param state: FLASHLIGHT_ON ? FLASHLIGHT_OFF
 * @return ESP_OK: on success.
 */
esp_err_t uwc_flashlight_set(bool state);

#endif
