#include "uwc_event.h"

uwcEvent_t uwc_event_cam_init(void) {
  uwcEventErr = uwc_cam_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_cam_deinit(void) {
  uwcEventErr = uwc_cam_deinit();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);
}

uwcEvent_t uwc_event_cam_grab(void) {
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event,
             "Camera is not initialized yet, initializing camera...");
    uwcEventErr = uwc_cam_init();
    if (uwcEventErr) {
      ESP_LOGE(uwc_tag_event, "Error during init camera!");
      return;
    }
  }

  uwcEventErr = uwc_cam_open();
  if (uwcEventErr) {
    ESP_LOGE(uwc_tag_event, "Camera get frame buffer failed!");
    return;
  }

  ESP_LOGI(uwc_tag_event, "Size of buffer: %u", uwcCamFb->len);
  size_t quotient = uwcCamFb->len / UDP_BUF_SIZE;
  size_t remainder = uwcCamFb->len % UDP_BUF_SIZE;
  unsigned int i = 0;
  for (i = 0; i < quotient; i++) {
    if (uwc_udp_send_raw((const void*)(uwcCamFb->buf + (i * UDP_BUF_SIZE)),
                         UDP_BUF_SIZE) < 0) {
      ESP_LOGE(uwc_tag_event, "Error in itteration: %i", i);
      uwc_cam_close();
      return;
    }
  }
  if (uwc_udp_send_raw((const void*)(uwcCamFb->buf + (i * UDP_BUF_SIZE)),
                       remainder) < 0) {
    uwc_cam_close();
    return;
  }

  uwc_cam_close();
}

uwcEvent_t uwc_event_cam_stream(void) {
  ESP_LOGI(uwc_tag_event, "Preparing streaming...");
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event,
             "Camera is not initialized yet, initializing camera...");
    uwcEventErr = uwc_cam_init();
    if (uwcEventErr) {
      ESP_LOGE(uwc_tag_event, "Error during init camera!");
      return;
    }
  }

  uwcEventErr = uwc_cam_open();
  if (uwcEventErr) {
    ESP_LOGE(uwc_tag_event, "Camera get frame buffer failed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Camera capture test oke!");
  uwc_cam_close();

  size_t quotient;
  size_t remainder;
  unsigned int i;
  const char endOfJpeg[2] = {0xff, 0xd9};

  for (;;) {  // Streaming data...
  start_stream:
    uwc_cam_open();
    quotient = uwcCamFb->len / UDP_BUF_SIZE;
    remainder = uwcCamFb->len % UDP_BUF_SIZE;
    for (i = 0; i < quotient; i++) {
      if (uwc_udp_send_raw((const void*)(uwcCamFb->buf + (i * UDP_BUF_SIZE)),
                           UDP_BUF_SIZE) < 0) {
        ESP_LOGE(uwc_tag_event, "Error in itteration: %i", i);
        uwc_udp_send_raw((const void*)&endOfJpeg, 2);
        uwc_cam_close();
        goto start_stream;
      }
    }
    if (uwc_udp_send_raw((const void*)(uwcCamFb->buf + (i * UDP_BUF_SIZE)),
                         remainder) < 0) {
      ESP_LOGE(uwc_tag_event, "Error in last itteration!");
      uwc_cam_close();
      continue;
    }
    uwc_cam_close();
  }
}
