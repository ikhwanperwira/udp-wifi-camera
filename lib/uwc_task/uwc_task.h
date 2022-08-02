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
// #include "uwc_udp.h"

// Type for uwc task.
typedef void uwcTask_t;

/**
 * @brief Task when OS is starting up.
 */
uwcTask_t uwc_startup_task();

/**
 * @brief Task for handle every command through UART.
 */
uwcTask_t uwc_uart_task(void *pvParameters);

#endif
