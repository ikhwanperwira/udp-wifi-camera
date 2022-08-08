#include "uwc_event.h"

uwcEvent_t uwc_event_led_init(void) {
  uwcEventErr = uwc_led_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("LED has been initialized!\n");
  }
}

uwcEvent_t uwc_event_led_set_on(void) {
  uwcEventErr = uwc_led_set(LED_ON);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("LED has been turned on!\n");
  }
}

uwcEvent_t uwc_event_led_set_off(void) {
  uwcEventErr = uwc_led_set(LED_OFF);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("LED has been turned off!\n");
  }
}
