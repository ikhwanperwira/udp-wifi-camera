#include <stdio.h>

#include "cam_cfg.h"
#include "driver/gpio.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "string.h"
#include "uart_cmd.h"

// WiFI Setup
#define WIFI_SSID "okelah"
#define WIFI_PASW "12345678"
#define WIFI_AUTH WIFI_AUTH_WPA2_PSK
#define MAX_TRY 10

// ESP32CAM AI-Thinker default LED PIN
#define ESP32CAM_PIN_BUILT_IN_LED GPIO_NUM_33
#define LED_ON 0
#define LED_OFF 1

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static const char PROG_TAG[] = "udp-wifi-camera";
static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (s_retry_num < MAX_TRY) {
      esp_wifi_connect();
      s_retry_num++;
      ESP_LOGI(PROG_TAG, "retry to connect to the AP");
    } else {
      xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }
    ESP_LOGI(PROG_TAG, "connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
    ESP_LOGI(PROG_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

void wifi_init_sta(void) {
  s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = WIFI_SSID,
              .password = WIFI_PASW,
              /* Setting a password implies station will connect to all security
               * modes including WEP/WPA. However these modes are deprecated and
               * not advisable to be used. Incase your Access point doesn't
               * support WPA2, these mode can be enabled by commenting below
               * line */
              .threshold.authmode = WIFI_AUTH,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(PROG_TAG, "wifi_init_sta finished.");

  /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
   * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
   * bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(PROG_TAG, "Connected to ap SSID:%s password:%s", WIFI_SSID,
             WIFI_PASW);
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(PROG_TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID,
             WIFI_PASW);
  } else {
    ESP_LOGE(PROG_TAG, "UNEXPECTED EVENT");
  }
}

void led_init() {
  gpio_pad_select_gpio(ESP32CAM_PIN_BUILT_IN_LED);

  if (gpio_set_direction(ESP32CAM_PIN_BUILT_IN_LED, GPIO_MODE_OUTPUT) ==
      ESP_OK) {
    uart_cmd_send("LED has been initialized!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED init failure!\n");
}

void turn_on_led() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 0) == ESP_OK) {
    uart_cmd_send("LED has been turned on!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED turn on failure!\n");
}

void turn_off_led() {
  if (gpio_set_level(ESP32CAM_PIN_BUILT_IN_LED, 1) == ESP_OK) {
    uart_cmd_send("LED has been turned off!\n");
    return;
  }

  ESP_LOGE(PROG_TAG, "LED turn off failure!\n");
}

static uint8_t is_camera_initialized = 0;

void camera_init() {
  if (is_camera_initialized) {
    ESP_LOGE(PROG_TAG, "Cam already initialized!\n");
    return;
  }

  if (esp_camera_init(&cam_cfg) == ESP_OK) {
    uart_cmd_send("Camera has been initialized!\n");
    is_camera_initialized = 1;
    return;
  }

  ESP_LOGE(PROG_TAG, "Cam init failure!\n");
}

void camera_deinit() {
  if (!is_camera_initialized) {
    ESP_LOGE(PROG_TAG, "Cam already deinitialized!\n");
    return;
  }

  if (esp_camera_deinit() == ESP_OK) {
    uart_cmd_send("Camera has been deinitialized!\n");
    is_camera_initialized = 0;
    return;
  }

  ESP_LOGE(PROG_TAG, "Camera deinit failure!\n");
}

void interface_show() {}

void app_main() {
  ESP_LOGI(PROG_TAG, "Start of program!");

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  if (uart_cmd_init(115200) == ESP_OK) {
    uart_cmd_send("\nUART CMD has been initialized!\n");
  } else {
    ESP_LOGE(PROG_TAG, "UART CMD init failure!");
  }

  uart_cmd_send("WiFi setup: (" WIFI_SSID "|" WIFI_PASW ")\n");

  for (;;) {
    uart_cmd_recv();
    uart_cmd_on("$led init\n", led_init, NULL);
    uart_cmd_on("$led on\n", turn_on_led, NULL);
    uart_cmd_on("$led off\n", turn_off_led, NULL);
    uart_cmd_on("$cam init\n", camera_init, NULL);
    uart_cmd_on("$cam deinit\n", camera_deinit, NULL);
    uart_cmd_on("$wifi init\n", wifi_init_sta, NULL);

    if (uart_cmd_is_data_match("$exit\n")) {
      uart_cmd_send("Exiting program...\n");
      break;
    } else if (uart_cmd_is_data_match("$ip\n")) {
      tcpip_adapter_ip_info_t ipInfo;
      tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
      char tmp[64];
      sprintf(tmp, "Current IP: " IPSTR "\n", IP2STR(&ipInfo.ip));
      uart_cmd_send(&tmp[0]);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(PROG_TAG, "End of program!");
}