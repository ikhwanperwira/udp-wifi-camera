#include "uwc_uart.h"

static char uwcUartBufRX[UART_BUF_SIZE];
static char uwcUartBufTX[UART_BUF_SIZE];
static bool isHandled = false;
static esp_err_t err;

esp_err_t uwc_uart_init(unsigned int baudrate) {
  uart_config_t uart_config = {
      .baud_rate = baudrate,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB,
  };
  err = uart_driver_install(UART_NUM_0, UART_BUF_SIZE * 2, 0, 0, NULL, 0);
  if (!err) {
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_TXD, UART_PIN_RXD, UART_PIN_RTS,
                 UART_PIN_CTS);
  }
  return err;
}

void uwc_uart_send(char* data) {
  strcpy(uwcUartBufTX, data);
  uart_write_bytes(UART_NUM_0, uwcUartBufTX, strlen(uwcUartBufTX));
}

unsigned int uwc_uart_recv(void) {
  unsigned int uwcUartBufRXLen = (unsigned int)uart_read_bytes(
      UART_NUM_0, uwcUartBufRX, UART_BUF_SIZE, 20 / portTICK_PERIOD_MS);
  uwcUartBufRX[uwcUartBufRXLen] = '\0';  // insert null-terminated string at end
                                         // because C string format rules.
  isHandled = 0;
  return uwcUartBufRXLen;
}

char* uwc_uart_get_data(void) { return &uwcUartBufRX[0]; }

bool uwc_uart_is_data_match(char* check) {
  return (bool)!strcmp(check, &uwcUartBufRX[0]);
}

void uwc_uart_on(char* data, void on_match(void), void on_unmatch(void)) {
  if (isHandled) {  // return if another event already handled this data.
    return;
  }
  if (!strcmp(data, uwcUartBufRX)) {
    if (on_match) {
      on_match();
      isHandled = 1;
    }
    return;
  } else {
    if (on_unmatch) {
      on_unmatch();
      isHandled = 1;
    }
    return;
  }
}
