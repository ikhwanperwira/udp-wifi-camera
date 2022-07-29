#include <stdio.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

// ESP32CAM AI-Thinker default LED PIN
#define ESP32CAM_PIN_BUILT_IN_LED GPIO_NUM_33
#define LED_ON 0
#define LED_OFF 1

// ESP32CAM AI-Thinker default UART PIN
#define UART_PIN_TXD GPIO_NUM_1
#define UART_PIN_RXD GPIO_NUM_3
#define UART_PIN_RTS UART_PIN_NO_CHANGE
#define UART_PIN_CTS UART_PIN_NO_CHANGE
#define UART_BUF_SIZE 1024

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

static const char PROG_TAG[] = "udp-wifi-camera";

static const camera_config_t camera_config = {
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

static const uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};

static char uartBufr[UART_BUF_SIZE];
static char uartBufw[UART_BUF_SIZE];

esp_err_t led_init() {
  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_LED);
  return gpio_set_direction(ESP32CAM_PIN_BUILT_IN_LED, GPIO_MODE_OUTPUT);
}

esp_err_t camera_init() { return esp_camera_init(&camera_config); }

esp_err_t uart_init() {
  esp_err_t err =
      uart_driver_install(UART_NUM_0, UART_BUF_SIZE * 2, 0, 0, NULL, 0);
  if (err & ESP_OK) {
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_TXD, UART_PIN_RXD, UART_PIN_RTS,
                 UART_PIN_CTS);
  }
  return err;
}

void app_main() {
  ESP_ERROR_CHECK_WITHOUT_ABORT(led_init());
  ESP_ERROR_CHECK_WITHOUT_ABORT(uart_init());
  ESP_ERROR_CHECK_WITHOUT_ABORT(camera_init());

  for (;;) {
    int uartBufrLen = uart_read_bytes(UART_NUM_0, uartBufr, UART_BUF_SIZE,
                                      20 / portTICK_PERIOD_MS);
    uartBufr[uartBufrLen] = '\0';  // insert null-terminated string at end
                                   // because C string format rules.
    uartBufw[0] = '\0';  // insert null-terminated string at first for safety.

    if (uartBufr[0] == '$') {
      if (!strcmp(&uartBufr[1], "led on\n")) {
        gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, LED_ON);
      } else if (!strcmp(&uartBufr[1], "led off\n")) {
        gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, LED_OFF);
      } else {
        sprintf(uartBufw, "%d\n", uartBufrLen);
        uart_write_bytes(UART_NUM_0, uartBufw, strlen(uartBufw));
      }
    }
  }
  ESP_LOGI(PROG_TAG, "End of program!");
}