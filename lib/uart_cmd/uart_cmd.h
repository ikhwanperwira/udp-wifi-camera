/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uart_cmd.
 *
 */

#ifndef __UART_CMD_H__
#define __UART_CMD_H__

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"

// ESP32CAM AI-Thinker default UART PIN, you maybe need configure this.
#define UART_PIN_TXD GPIO_NUM_1
#define UART_PIN_RXD GPIO_NUM_3
#define UART_PIN_RTS UART_PIN_NO_CHANGE
#define UART_PIN_CTS UART_PIN_NO_CHANGE
#define UART_BUF_SIZE 1024

/**
 * @brief Initializing uart_cmd.
 * @param baudrate: Baud rate speed, normally it must be 115200.
 * @return
 *     ESP_OK:       Success.
 *     !ESP_OK:      Fail.
 */
esp_err_t uart_cmd_init(uint32_t baudrate);

/**
 * @brief Send data.
 * @param data: Data to send.
 * @return
 *  nothing.
 */
void uart_cmd_send(char* data);

/**
 * @brief  Receive data. Use uart_cmd_on() for handling data received,
 * or access read buffer with uart_cmd_get_data().
 * @return
 *  Total bytes received.
 */
unsigned int uart_cmd_recv();

/**
 * @brief Get data received that stored in read buffer after uart_cmd_recv()
 * received. library.
 * @return
 *  Pointer to the data.
 */
char* uart_cmd_get_data();

/**
 * @brief Get data received that stored in read buffer after uart_cmd_recv() is
 * called.
 * @param check: Check if data received is match with this parameter.
 * @return bool.
 *  Pointer to the data.
 */
int uart_cmd_is_data_match(char* check);

/**
 * @brief Handling data with callback.
 * @param data: Data to compare.
 * @param on_match: Callback do something if data received is match, set to
 * NULL if not needed.
 * @param on_unmatch: Callback do something if data received is unmatch, set
 * to NULL if not needed.
 * @return
 *  nothing.
 */
void uart_cmd_on(char* data, void on_match(), void on_unmatch());

#endif