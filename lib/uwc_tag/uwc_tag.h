/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_tag.
 *
 */

#ifndef __UWC_TAG_H__
#define __UWC_TAG_H__

/**
 * @brief udp-wifi-camera tag for ESP_LOG usage. This is general tag.
 */
extern const char uwc_tag[];

/**
 * @brief LED tag for ESP_LOG usage.
 */
extern const char uwc_tag_led[];

/**
 * @brief Camera tag for ESP_LOG usage.
 */
extern const char uwc_tag_cam[];

/**
 * @brief NVS tag for ESP_LOG usage.
 */
extern const char uwc_tag_nvs[];

/**
 * @brief UART tag for ESP_LOG usage.
 */
extern const char uwc_tag_uart[];

/**
 * @brief WiFI tag for ESP_LOG usage.
 */
extern const char uwc_tag_wifi[];

/**
 * @brief UDP tag for ESP_LOG usage.
 */
extern const char uwc_tag_udp[];

/**
 * @brief Event tag for ESP_LOG usage.
 */
extern const char uwc_tag_event[];

/**
 * @brief Task tag for ESP_LOG usage.
 */
extern const char uwc_tag_task[];

#endif
