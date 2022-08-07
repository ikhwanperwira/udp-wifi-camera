/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * udp-wifi-camera event.
 *
 */

#ifndef __UWC_EVENT_H__
#define __UWC_EVENT_H__

#include "esp_err.h"
#include "uwc_cam.h"
#include "uwc_eolremover.h"
#include "uwc_led.h"
#include "uwc_nvs.h"
#include "uwc_tag.h"
#include "uwc_task.h"
#include "uwc_uart.h"
#include "uwc_udp.h"
#include "uwc_wifi.h"
#include "zlib.h"

// Type for uwcEvent_t
typedef void uwcEvent_t;

// Error variable that will be usage by any event.
esp_err_t uwcEventErr;

/**
 * @brief initializing built-in LED.
 */
uwcEvent_t uwc_event_led_init(void);

/**
 * @brief Set on LED.
 */
uwcEvent_t uwc_event_led_set_on(void);

/**
 * @brief Set off LED.
 */
uwcEvent_t uwc_event_led_set_off(void);

/**
 * @brief Init camera.
 */
uwcEvent_t uwc_event_cam_init(void);

/**
 * @brief Deinit camera.
 */
uwcEvent_t uwc_event_cam_deinit(void);

/**
 * @brief Grab current frame buffer and send through UDP socket.
 */
uwcEvent_t uwc_event_cam_grab(void);

/**
 * @brief Stream frame buffer through UDP socket.
 */
uwcEvent_t uwc_event_cam_stream(void);

/**
 * @brief Init NVS.
 */
uwcEvent_t uwc_event_nvs_init(void);

/**
 * @brief Open NVS with using namespace "uwc".
 */
uwcEvent_t uwc_event_nvs_open(void);

/**
 * @brief Read SSID from NVS.
 */
uwcEvent_t uwc_event_nvs_read_ssid(void);

/**
 * @brief Write SSID from NVS.
 */
uwcEvent_t uwc_event_nvs_write_ssid(void);

/**
 * @brief Erase SSID from NVS.
 */
uwcEvent_t uwc_event_nvs_erase_ssid(void);

/**
 * @brief Read PASW from NVS.
 */
uwcEvent_t uwc_event_nvs_read_pasw(void);

/**
 * @brief Write PASW from NVS.
 */
uwcEvent_t uwc_event_nvs_write_pasw(void);

/**
 * @brief Erase PASW from NVS.
 */
uwcEvent_t uwc_event_nvs_erase_pasw(void);

/**
 * @brief Commit changes WiFi auth into NVS.
 */
uwcEvent_t uwc_event_nvs_commit_auth(void);

/**
 * @brief Close NVS with namespace "uwc".
 */
uwcEvent_t uwc_event_nvs_close(void);

/**
 * @brief Init WiFI.
 */
uwcEvent_t uwc_event_wifi_init(void);

/**
 * @brief Setup new SSID and new PASW with UART input.
 */
uwcEvent_t uwc_event_wifi_setup_with_uart(void);

/**
 * @brief Setup new SSID and new PASW with UDP input.
 */
uwcEvent_t uwc_event_wifi_setup_with_udp(void);

/**
 * @brief Show WiFi connected info.
 */
uwcEvent_t uwc_event_wifi_info(void);

/**
 * @brief Deinit WiFI.
 */
uwcEvent_t uwc_event_wifi_deinit(void);

/**
 * @brief Init UDP (using gateway destination).
 */
uwcEvent_t uwc_event_udp_init(void);

/**
 * @brief Reinit UDP (applying new destination).
 */
uwcEvent_t uwc_event_udp_reinit(void);

/**
 * @brief Send UDP from UART
 */
uwcEvent_t uwc_event_udp_send(void);

/**
 * @brief Receive UDP echo to UART
 */
uwcEvent_t uwc_event_udp_recv(void);

/**
 * @brief Setup UDP destinastion with UART input.
 */
uwcEvent_t uwc_event_udp_setup_with_uart(void);

/**
 * @brief Setup UDP destinastion with UDP input.
 */
uwcEvent_t uwc_event_udp_setup_with_udp(void);

/**
 * @brief Interrupt UDP handshaking while initializing.
 */
uwcEvent_t uwc_event_udp_interrupt_handshake(void);

/**
 * @brief Kill UDP task.
 */
uwcEvent_t uwc_event_udp_kill(void);

/**
 * @brief Kill UDP task in its task itself.
 */
uwcEvent_t uwc_event_udp_killself(void);

#endif
