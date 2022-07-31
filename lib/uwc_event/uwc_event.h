/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * udp wifi camera event.
 *
 */

#ifndef __UWC_EVENT_H__
#define __UWC_EVENT_H__

#include "cam_cfg.h"
#include "log_tag.h"
#include "uart_cmd.h"
#include "wifi_cfg.h"

// ESP32CAM AI-Thinker default LED PIN
#define ESP32CAM_PIN_BUILT_IN_LED GPIO_NUM_33
#define LED_ON 0
#define LED_OFF 1

/**
 * @brief initializing built-in LED.
 */
void led_init();

/**
 * @brief Set on LED.
 */
void led_set_on();

/**
 * @brief Set off LED.
 */
void led_set_off();

/**
 * @brief Initializing camera.
 */
void camera_init();

/**
 * @brief Deinitializing camera.
 */
void camera_deinit();

/**
 * @brief Initializing WiFI STA.
 */
void wifi_init_sta();

/**
 * @brief Deinitializing WiFI STA.
 */
void wifi_deinit_sta();

/**
 * @brief Show current IP after WiFI STA initialized.
 */
void wifi_ip_sta_show();

/**
 * @brief Initializing NVS.
 */
void nvs_init();

#endif