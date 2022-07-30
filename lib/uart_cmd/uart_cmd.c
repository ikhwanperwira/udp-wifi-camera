#include "uart_cmd.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"

static char uartCmdBufr[UART_BUF_SIZE];
static char uartCmdBufw[UART_BUF_SIZE];

esp_err_t uart_cmd_init(uint32_t baudrate) {
  uart_config_t uart_config = {
      .baud_rate = baudrate,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB,
  };
  esp_err_t err =
      uart_driver_install(UART_NUM_0, UART_BUF_SIZE * 2, 0, 0, NULL, 0);
  if (err == ESP_OK) {
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_TXD, UART_PIN_RXD, UART_PIN_RTS,
                 UART_PIN_CTS);
  }
  return err;
}

void uart_cmd_send(char* data) {
  strcpy(uartCmdBufw, data);
  uart_write_bytes(UART_NUM_0, uartCmdBufw, strlen(uartCmdBufw));
}

unsigned int uart_cmd_recv() {
  unsigned int uartCmdBufrLen = (unsigned int)uart_read_bytes(
      UART_NUM_0, uartCmdBufr, UART_BUF_SIZE, 20 / portTICK_PERIOD_MS);
  uartCmdBufr[uartCmdBufrLen] = '\0';  // insert null-terminated string at end
                                       // because C string format rules.
  return uartCmdBufrLen;
}

char* uart_cmd_get_data() { return uartCmdBufr; }

int uart_cmd_is_data_match(char* check) {
  return !strcmp(check, &uartCmdBufr[0]);
}

void uart_cmd_on(char* data, void on_match(), void on_unmatch()) {
  if (!strcmp(data, uartCmdBufr)) {
    if (on_match) {
      on_match();
    }
    return;
  } else {
    if (on_unmatch) {
      on_unmatch();
    }
    return;
  }
}