/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_led.
 *
 */

#ifndef __UWC_LED_H__
#define __UWC_LED_H__

// Predefined
#define ESP32CAM_PIN_BUILT_IN_LED GPIO_NUM_33
#define LED_ON 0
#define LED_OFF 1

// Dependencies
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "stdbool.h"
#include "uwc_tag.h"

/**
 * @brief LED initialization status.
 */
extern bool uwcIsLedInit;

/**
 * @brief Initializing built-in LED ESP32 Camera AI-Thinker mode output.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_led_init(void);

/**
 * @brief Set LED state.
 * @param state: LED_ON ? LED_OFF
 * @return ESP_OK: on success.
 */
esp_err_t uwc_led_set(bool state);

#endif
