#include "uwc_event.h"

static bool isUdpTaskStart = false;

uwcEvent_t uwc_event_udp_kill(void) {
  if (!isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "Already killed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing UDP task...");
  uwcUdpTimeoutCount = 1;
  uwcUdpIsInit = false;
  isUdpTaskStart = false;
  vTaskDelete(uwc_task_handle_udp);
  ESP_LOGI(uwc_tag_event, "Kill UDP task done!");
}

uwcEvent_t uwc_event_udp_killself(void) {
  if (!isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "Already killed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing my self...");
  uwcUdpTimeoutCount = 1;
  uwcUdpIsInit = false;
  isUdpTaskStart = false;
}

uwcEvent_t uwc_event_udp_init(void) {
  if (isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "UDP task already started!");
    return;
  }

  while (!uwcWifiIsInit) {
    ESP_LOGW(uwc_tag_event,
             "WiFi is not initialized yet, initializing wifi...");
    uwc_event_wifi_init();
    if (!uwcEventErr) {
      break;
    }
  }

  if (uwcUdpIsInit) {
    uwc_event_udp_kill();
  }

  char gw[16];
  sprintf(gw, IPSTR, IP2STR(&uwcWifiIpInfo.gw));
  strcpy(SERV_IPV4, gw);
  SERV_PORT = 39876;

  ESP_LOGI(uwc_tag_event, "Creating UDP task...");
  xTaskCreate(uwc_task_udp, "uwc_task_udp", 32768, NULL, 24,
              &uwc_task_handle_udp);
  ESP_LOGI(uwc_tag_event, "Create UDP task done!");
  isUdpTaskStart = true;
}

uwcEvent_t uwc_event_udp_reinit(void) {
  if (isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "UDP task already started!");
    return;
  }

  while (!uwcWifiIsInit) {
    ESP_LOGW(uwc_tag_event,
             "WiFi is not initialized yet, initializing wifi...");
    uwc_event_wifi_init();
    if (!uwcEventErr) {
      break;
    }
  }

  if (uwcUdpIsInit) {
    uwc_event_udp_kill();
  }

  ESP_LOGI(uwc_tag_event, "Creating UDP task...");
  xTaskCreate(uwc_task_udp, "uwc_task_udp", 32768, NULL, 24,
              &uwc_task_handle_udp);
  ESP_LOGI(uwc_tag_event, "Create UDP task done!");
  isUdpTaskStart = true;
}

uwcEvent_t uwc_event_udp_send(void) {
  char output[16];
  for (;;) {
    uwc_uart_input("msg: ", output, false, false);
    if (!strcmp(output, "$exit\n")) {
      uwc_uart_flush();
      ESP_LOGI(uwc_tag_event, "Exited from UDP send!");
      break;
    }
    uwc_udp_send(output);
  }
}

uwcEvent_t uwc_event_udp_recv(void) {
  int len = uwc_udp_recv();
  char* input = uwc_udp_get_data();
  char display[32];
  sprintf(display, "(%i, %i)|%s", len, strlen(input), input);
  uwc_uart_send(display);
}

uwcEvent_t uwc_event_udp_setup_with_uart(void) {
  char ipv4Str[32];
  char portStr[8];
  char confirm[4];
  uwc_uart_input("Enter IPV4: ", ipv4Str, true, true);
  uwc_uart_input("Enter PORT: ", portStr, true, true);
  uwc_uart_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    ESP_LOGW(uwc_tag_event, "UDP setup aborted!");
    return;
  }

  if (strlen(ipv4Str) > 16 || strlen(portStr) > 16) {
    ESP_LOGE(uwc_tag_event, "Invalid input, aborted!");
    return;
  }

  strcpy(SERV_IPV4, ipv4Str);
  SERV_PORT = (u16_t)atoi(portStr);

  uwc_uart_send("UDP setup has been updated!\n");
}

uwcEvent_t uwc_event_udp_setup_with_udp(void) {
  char ipv4Str[32];
  char portStr[8];
  char confirm[4];
  uwc_udp_input("Enter IPV4: ", ipv4Str, true, true);
  uwc_udp_input("Enter PORT: ", portStr, true, true);
  uwc_udp_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    uwc_udp_send("UDP setup aborted!\n");
    return;
  }

  if (strlen(ipv4Str) > 16 || strlen(portStr) > 16) {
    uwc_udp_send("Invalid input, aborted!\n");
    return;
  }

  strcpy(SERV_IPV4, ipv4Str);
  SERV_PORT = (u16_t)atoi(portStr);

  uwc_udp_send("UDP setup has been updated!\n");
}
