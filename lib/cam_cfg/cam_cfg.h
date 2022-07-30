/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uart_cmd.
 *
 */

#ifndef __CAM_CFG_H__
#define __CAM_CFG_H__
#include "esp_camera.h"

// CAMERA OV2640 PIN
#define CAM_PIN_PWDN GPIO_NUM_32
#define CAM_PIN_RESET GPIO_NUM_NC  // software reset will be performed
#define CAM_PIN_XCLK GPIO_NUM_0
#define CAM_PIN_SIOD GPIO_NUM_26
#define CAM_PIN_SIOC GPIO_NUM_27
#define CAM_PIN_D7 GPIO_NUM_35
#define CAM_PIN_D6 GPIO_NUM_34
#define CAM_PIN_D5 GPIO_NUM_39
#define CAM_PIN_D4 GPIO_NUM_36
#define CAM_PIN_D3 GPIO_NUM_21
#define CAM_PIN_D2 GPIO_NUM_19
#define CAM_PIN_D1 GPIO_NUM_18
#define CAM_PIN_D0 GPIO_NUM_5
#define CAM_PIN_VSYNC GPIO_NUM_25
#define CAM_PIN_HREF GPIO_NUM_23
#define CAM_PIN_PCLK GPIO_NUM_22

/**
 * @brief Location of camera config.
 */
static const camera_config_t cam_cfg = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,
    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 16000000,  // EXPERIMENTAL: Set to 16MHz on ESP32-S2 or
                               // ESP32-S3 to enable EDMA mode
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,  // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size =
        FRAMESIZE_UXGA,  // QQVGA-QXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12,  // 0-63 lower number means higher quality
    .fb_count =
        1,  // if more than one, i2s runs in continuous mode. Use only with JPEG
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY  // CAMERA_GRAB_LATEST. Sets when
                                         // buffers should be filled
};

#endif