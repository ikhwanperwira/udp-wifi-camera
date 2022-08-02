#include "uwc_event.h"

uwcEvent_t uwc_event_cam_init(void) {
  uwcEventErr = uwc_cam_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_cam_deinit(void) {
  uwcEventErr = uwc_cam_deinit();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}
