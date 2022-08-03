#include "uwc_event.h"

static bool isUdpTaskStart = false;

uwcEvent_t uwc_event_udp_kill(void) {
  if (!isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "Already killed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing UDP task...");
  timeoutCounter = 1;
  isUdpTaskStart = false;
  isUdpInit = false;
  vTaskDelete(uwc_task_handle_udp);
  ESP_LOGI(uwc_tag_event, "Kill UDP task done!");
}

uwcEvent_t uwc_event_udp_killself(void) {
  if (!isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "Already killed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing my self...");
  timeoutCounter = 1;
  isUdpTaskStart = false;
  isUdpInit = false;
}

uwcEvent_t uwc_event_udp_init(void) {
  while (!uwcIsWifiInit) {
    ESP_LOGW(uwc_tag_event,
             "WiFi is not initialized yet, initializing wifi...");
    uwc_event_wifi_init();
    if (!uwcEventErr) {
      break;
    }
  }

  if (isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "UDP task already started!");
    return;
  }

  if (isUdpInit) {
    uwc_event_udp_kill();
  }

  if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &uwcIPInfo)) {
    ESP_LOGE(uwc_tag_event, "Error getting IP info!");
    return;
  }

  char gw[16];
  sprintf(gw, IPSTR, IP2STR(&uwcIPInfo.gw));
  strcpy(SERV_IPV4, gw);
  SERV_PORT = 8888;

  ESP_LOGI(uwc_tag_event, "Creating UDP task...");
  xTaskCreate(uwc_task_udp, "uwc_task_udp", 4096, NULL, 7,
              &uwc_task_handle_udp);
  ESP_LOGI(uwc_tag_event, "Create UDP task done!");
  isUdpTaskStart = true;
}

uwcEvent_t uwc_event_udp_reinit(void) {
  while (!uwcIsWifiInit) {
    ESP_LOGW(uwc_tag_event,
             "WiFi is not initialized yet, initializing wifi...");
    uwc_event_wifi_init();
    if (!uwcEventErr) {
      break;
    }
  }

  if (isUdpTaskStart) {
    ESP_LOGW(uwc_tag_event, "UDP task already started!");
    return;
  }

  if (isUdpInit) {
    uwc_event_udp_kill();
  }

  ESP_LOGI(uwc_tag_event, "Creating UDP task...");
  xTaskCreate(uwc_task_udp, "uwc_task_udp", 4096, NULL, 7,
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
  char portStrng[6];
  uwc_uart_input("Enter IPV4: ", SERV_IPV4, true, true);
  uwc_uart_input("Enter PORT: ", portStrng, true, true);

  SERV_PORT = (u16_t)atoi(portStrng);

  uwc_uart_send("UDP setup has been updated!\n");
}

uwcEvent_t uwc_event_udp_setup_with_udp(void) {
  ESP_LOGI(uwc_tag_event, "Setting UDP destinastion through UDP...");
  char portStrng[6];
  uwc_udp_input("Enter IPV4: ", SERV_IPV4, true, true);
  uwc_udp_input("Enter PORT: ", portStrng, true, true);

  SERV_PORT = (u16_t)atoi(portStrng);

  uwc_udp_send("UDP setup has been updated!\n");
  ESP_LOGI(uwc_tag_event, "Setting UDP done!");
}

uwcEvent_t uwc_event_udp_interrupt_handshake(void) {
  interruptHandshake = true;
}
