#include "uwc_event.h"

uwcEvent_t uwc_event_flashlight_init(void) {
  uwcEventErr = uwc_flashlight_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("Flashlight has been initialized!\n");
  }
}

uwcEvent_t uwc_event_flashlight_set_on(void) {
  uwcEventErr = uwc_flashlight_set(FLASHLIGHT_ON);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("Flashlight has been turned on!\n");
  }
}

uwcEvent_t uwc_event_flashlight_set_off(void) {
  uwcEventErr = uwc_flashlight_set(FLASHLIGHT_OFF);
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("Flashlight has been turned off!\n");
  }
}
