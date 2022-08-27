/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_wifi.
 *
 */

#ifndef __UWC_WIFI_H__
#define __UWC_WIFI_H__

#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "uwc_tag.h"

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

// WiFI Setup
#define MAX_TRY 20

extern char WIFI_SSID[32];
extern char WIFI_PASW[32];
esp_netif_t* uwcWifiStaNetif;
esp_netif_ip_info_t uwcWifiIpInfo;  // IP when connected to AP.
extern bool uwcWifiIsInit;          // WiFI initialization status.

/**
 * @brief WiFi STA init.
 */
esp_err_t uwc_wifi_init_sta(void);

/**
 * @brief WiFi STA deinit (still bug).
 */
esp_err_t uwc_wifi_deinit_sta(void);

#endif
