/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_wifi_cfg.
 *
 */

#ifndef __UWC_WIFI_CFG_H__
#define __UWC_WIFI_CFG_H__

#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "string.h"
#include "uwc_tag.h"

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

// WiFI Setup
#define WIFI_AUTH WIFI_AUTH_WPA2_PSK
#define MAX_TRY 10
extern char WIFI_SSID[32];
extern char WIFI_PASW[32];

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_wifi_event_group;

extern int s_retry_num;

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                   void* event_data);

#endif