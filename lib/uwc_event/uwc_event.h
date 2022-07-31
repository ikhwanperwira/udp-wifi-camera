/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * udp wifi camera event.
 *
 */

#ifndef __UWC_EVENT_H__
#define __UWC_EVENT_H__

#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "uwc_cam_cfg.h"
#include "uwc_eol_remover.h"
#include "uwc_tag.h"
#include "uwc_uart.h"
#include "uwc_wifi_cfg.h"

#define ESP32CAM_PIN_BUILT_IN_LED GPIO_NUM_33
#define LED_ON 0
#define LED_OFF 1

/**
 * @brief initializing built-in LED.
 */
void uwc_led_init();

/**
 * @brief Set on LED.
 */
void uwc_led_set_on();

/**
 * @brief Set off LED.
 */
void uwc_led_set_off();

/**
 * @brief Initializing camera.
 */
void uwc_cam_init();

/**
 * @brief Deinitializing camera.
 */
void uwc_cam_deinit();

/**
 * @brief Initializing WiFI STA.
 */
void uwc_wifi_init_sta();

/**
 * @brief Deinitializing WiFI STA.
 */
void uwc_wifi_deinit_sta();

/**
 * @brief Setting new SSID and new PASW into Non-Volatile Storage.
 */
void uwc_wifi_set();

/**
 * @brief Show current IP after WiFI STA initialized.
 */
void uwc_wifi_ip();

/**
 * @brief Initializing NVS.
 */
void uwc_nvs_init();

/**
 * @brief NVS open "uwc" storage.
 */
void uwc_nvs_open();

/**
 * @brief NVS read SSID in "uwc" storage.
 */
void uwc_nvs_read_ssid();

/**
 * @brief NVS write SSID  in "uwc" storage.
 */
void uwc_nvs_write_ssid();

/**
 * @brief NVS erase WIFI_SSID key in "uwc" storage.
 */
void uwc_nvs_erase_ssid();

/**
 * @brief NVS read PASW in "uwc" storage.
 */
void uwc_nvs_read_pasw();

/**
 * @brief NVS write SSID  in "uwc" storage.
 */
void uwc_nvs_write_pasw();

/**
 * @brief NVS erase WIFI_PASW key in "uwc" storage.
 */
void uwc_nvs_erase_pasw();

/**
 * @brief NVS commit wifi credential data in "uwc" storage.
 */
void uwc_nvs_commit();

/**
 * @brief NVS close "uwc" storage.
 */
void uwc_nvs_close();

#endif