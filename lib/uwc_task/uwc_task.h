/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * udp-wifi-camera event.
 *
 */

#ifndef __UWC_TASK_H__
#define __UWC_TASK_H__

#include "freertos/projdefs.h"
#include "uwc_event.h"

// Task start status.
extern bool uwcCamIsTaskStart;
extern bool uwcUdpIsTaskStart;

// Type for uwc task.
typedef void uwcTask_t;
typedef void *uwcTaskHandle_t;

/**
 * @brief Task when OS is starting up.
 */
uwcTask_t uwc_task_startup(void);

/**
 * @brief Task for handle every command through UART.
 */
uwcTask_t uwc_task_uart(void *uartFlag);

/**
 * @brief Task for handle every command through UDP.
 */
uwcTask_t uwc_task_udp(void *udpFlag);

/**
 * @brief Handle for uwc_task_udp.
 */
uwcTaskHandle_t uwc_task_handle_udp;

/**
 * @brief Task for camera streaming.
 */
uwcTask_t uwc_task_cam(void *camFlag);

/**
 * @brief Handle for uwc_task_cam
 */
uwcTaskHandle_t uwc_task_handle_cam;

#endif
