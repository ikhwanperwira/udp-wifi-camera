/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_uart.
 *
 */

#ifndef __UWC_UART_H__
#define __UWC_UART_H__

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"

// ESP32CAM AI-Thinker default UART PIN, you maybe need configure this.
#define UART_PIN_TXD GPIO_NUM_1
#define UART_PIN_RXD GPIO_NUM_3
#define UART_PIN_RTS UART_PIN_NO_CHANGE
#define UART_PIN_CTS UART_PIN_NO_CHANGE
#define UART_BUF_SIZE 1024

/**
 * @brief Initializing uwc_uart.
 * @param baudrate: Baud rate speed, normally it must be 115200.
 * @return
 *     ESP_OK:       Success.
 *     !ESP_OK:      Fail.
 */
esp_err_t uwc_uart_init(unsigned int baudrate);

/**
 * @brief Send data.
 * @param data: Data to send.
 * @return
 *  nothing.
 */
void uwc_uart_send(char* data);

/**
 * @brief  Receive data. Use uwc_uart_on() for handling data received,
 * or access read buffer with uwc_uart_get_data().
 * @return
 *  Total bytes received.
 */
unsigned int uwc_uart_recv(void);

/**
 * @brief Get data received that stored in read buffer after uwc_uart_recv()
 * is called.
 * @return
 *  Pointer to the data.
 */
char* uwc_uart_get_data(void);

/**
 * @brief Get data received that stored in read buffer after uwc_uart_recv() is
 * called.
 * @param check: Check if data received is match with this parameter.
 * @return bool.
 */
bool uwc_uart_is_data_match(char* check);

/**
 * @brief Handling data with callback after uwc_uart_recv() is called.
 * @param data: Data to compare.
 * @param on_match: Callback do something if data received is match, set to
 * NULL if not needed.
 * @param on_unmatch: Callback do something if data received is unmatch, set
 * to NULL if not needed.
 * @return
 *  nothing.
 */
void uwc_uart_on(char* data, void on_match(), void on_unmatch());

#endif
