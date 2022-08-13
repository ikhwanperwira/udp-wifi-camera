/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_cam.
 *
 */

#ifndef __UWC_CAM_H__
#define __UWC_CAM_H__

// Dependencies
#include "esp_camera.h"
#include "esp_err.h"
#include "esp_log.h"
#include "uwc_tag.h"

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

extern bool uwcCamIsInit;  // Camera initialization status.
extern camera_fb_t* uwcCamFb;
extern char uwcCamFrameSize[8];
extern char uwcCamJpegCompression[8];

/**
 * @brief Initializing camera.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_cam_init(void);

/**
 * @brief Deinitializing camera.
 * @return ESP_OK: on success.
 */
esp_err_t uwc_cam_deinit(void);

/**
 * @brief Open camera, get uwcCamFb frame buffer pointer available.
 */
esp_err_t uwc_cam_open(void);

/**
 * @brief Close camera, make sure call this before reuse uwcCamFb.
 */
void uwc_cam_close(void);

#endif
