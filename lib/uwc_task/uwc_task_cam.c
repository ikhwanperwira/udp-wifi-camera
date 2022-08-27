#include "uwc_task.h"

uwcTaskHandle_t uwc_task_handle_cam = NULL;
bool uwcCamIsTaskStart = false;

uwcTask_t uwc_task_cam(void *camFlag) {
  for (;;) {                                // streaming video...
    camera_fb_t *fb = esp_camera_fb_get();  // Get frame buffer
    register uint8_t *from = fb->buf;
    register size_t toSend = fb->len;

    while (toSend > 0) {  // Sending packet per packet...
      register size_t sendSize = toSend > UDP_BUF_SIZE ? UDP_BUF_SIZE : toSend;
      uwc_udp_send_raw(from, sendSize);
      toSend -= sendSize;
      from += sendSize;
    }
    esp_camera_fb_return(fb);
  }
}