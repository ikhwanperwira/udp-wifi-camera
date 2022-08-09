#include "uwc_event.h"

bool uwcCamIsTaskStart = false;

uwcEvent_t uwc_event_cam_init(void) {
  if (uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event, "Camera already initialized!");
    return;
  }

  uwc_event_nvs_init();
  uwc_event_nvs_open();
  uwc_event_nvs_read_framesize();
  uwc_event_nvs_read_jpeglevel();
  uwcEventErr = uwc_cam_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(uwcEventErr);

  if (uwcUdpIsInit) {
    if (uwcEventErr) {
      uwc_udp_send("Camera init failed!\n");
      return;
    }
    uwc_udp_send("Frame size config: ");
    uwc_udp_send(uwcCamFrameSize);
    uwc_udp_send("\n");
    uwc_udp_send("JPEG compression config: ");
    uwc_udp_send(uwcCamJpegCompression);
    uwc_udp_send("\n");
    uwc_udp_send("Camera has been initialized!\n");
  }
}

uwcEvent_t uwc_event_cam_deinit(void) {
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event, "Camera already deinitialized!");
    return;
  }

  uwc_event_cam_kill();

  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("Camera has been deinitialized!\n");
  }
}

uwcEvent_t uwc_event_cam_grab(void) {
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event,
             "Camera is not initialized yet, initializing camera...");
    uwc_event_cam_init();
    if (uwcEventErr) {
      ESP_LOGE(uwc_tag_event, "Error during init camera!");
      return;
    }
  }

  // char compressedFb[16384];

  // camera_fb_t *fb = esp_camera_fb_get();

  // z_stream deflstream;

  // deflstream.zalloc = Z_NULL;
  // deflstream.zfree = Z_NULL;
  // deflstream.opaque = Z_NULL;

  // // Prepare compression
  // deflstream.avail_in = (uInt)fb->len;
  // deflstream.next_in = (Bytef *)fb->buf;
  // deflstream.avail_out = (uInt)sizeof(compressedFb);
  // deflstream.next_out = (Bytef *)compressedFb;
  // esp_camera_fb_return(fb);

  // // Actual compression
  // deflateInit(&deflstream, Z_DEFAULT_COMPRESSION);
  // deflate(&deflstream, Z_FINISH);
  // deflateEnd(&deflstream);

  // char *from = &compressedFb[0];
  // size_t toSend = (size_t)deflstream.total_out;
  // ESP_LOGI(uwc_tag_event, "Compressed:Actual = %u:%u",
  //          (size_t)deflstream.total_out,
  //          (size_t)deflstream.total_in);  // Just for debugging

  uwcEventErr = uwc_cam_open();
  if (uwcEventErr) {
    ESP_LOGE(uwc_tag_event, "Camera get frame buffer failed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Camera capture test OK with size: %u",
           uwcCamFb->len);

  ESP_LOGI(uwc_tag_event, "Camera capture OK with size: %u", uwcCamFb->len);

  register uint8_t *from = uwcCamFb->buf;
  register size_t toSend = uwcCamFb->len;

  for (int i = 0; toSend > 0; i++) {
    size_t sendSize = toSend > UDP_BUF_SIZE ? UDP_BUF_SIZE : toSend;
    while (uwc_udp_send_raw(from, sendSize) < 0) {
      ESP_LOGW(uwc_tag_event, "Retry in itteration: %i", i);
    }
    toSend -= sendSize;
    from += sendSize;
  }
  uwc_cam_close();
}

uwcEvent_t uwc_event_cam_stream(void) {
  if (uwcCamIsTaskStart) {
    ESP_LOGW(uwc_tag_event, "Camera already streaming!");
    return;
  }

  ESP_LOGI(uwc_tag_event, "Preparing streaming...");
  if (!uwcCamIsInit) {
    ESP_LOGW(uwc_tag_event,
             "Camera is not initialized yet, initializing camera...");
    uwc_event_cam_init();
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
  ESP_LOGI(uwc_tag_event, "Camera capture test OK with size: %u",
           uwcCamFb->len);
  uwc_cam_close();

  ESP_LOGI(uwc_tag_event, "Creating camera stream task...");
  xTaskCreate(uwc_task_camera, "uwc_task_camera", 16384, NULL,
              configMAX_PRIORITIES, &uwc_task_handle_camera);
  ESP_LOGI(uwc_tag_event, "Create camera stream task done!");
  uwc_udp_set_timeout(0, 0);
  uwcCamIsTaskStart = true;
}

uwcEvent_t uwc_event_cam_kill(void) {
  if (!uwcCamIsTaskStart) {
    ESP_LOGW(uwc_tag_event, "Already killed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing camera stream task...");
  vTaskDelete(uwc_task_handle_camera);
  uwcEventErr = uwc_cam_deinit();
  if (uwcEventErr) {
    ESP_LOGE(uwc_tag_event, "Camera kill failed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Killing camera stream task done!");
  if (uwcUdpIsInit && !uwcEventErr) {
    uwc_udp_send("Camera task has been killed!\n");
  }
  uwc_udp_set_timeout(1, 0);
  uwcCamIsTaskStart = false;
}

uwcEvent_t uwc_event_cam_setup_with_uart(void) {
  char confirm[4];
  char framesize[8];
  char jpeglevel[8];
  uwc_uart_input(
      "Frame Size:\n"
      "  0: 96x96\n"
      "  1: 160x120\n"
      "  2: 176x144\n"
      "  3: 240x176\n"
      "  4: 240x240\n"
      "  5: 320x240\n"
      "  6: 400x296\n"
      "  7: 480x320\n"
      "  8: 640x480\n"
      "  9: 800x600\n"
      " 10: 1024x768\n"
      " 11: 1280x720\n"
      " 12: 1280x1024\n"
      " 13: 1600x1200\n"
      "Choose frame size (0-13): ",
      framesize, true, true);
  uwc_uart_input("JPEG level compression (1-63): ", jpeglevel, true, true);
  uwc_uart_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    ESP_LOGW(uwc_tag_event, "Camera setup aborted!");
    return;
  }

  strcpy(uwcCamFrameSize, framesize);
  strcpy(uwcCamJpegCompression, jpeglevel);

  uwc_event_nvs_write_framesize();
  uwc_event_nvs_write_jpeglevel();
  uwc_event_nvs_commit_auth();
  uwc_uart_send("Camera setup has been updated!\n");
}

uwcEvent_t uwc_event_cam_setup_with_udp(void) {
  char confirm[4];
  uwc_udp_input(
      "Frame Size:\n"
      "  0: 96x96\n"
      "  1: 160x120\n"
      "  2: 176x144\n"
      "  3: 240x176\n"
      "  4: 240x240\n"
      "  5: 320x240\n"
      "  6: 400x296\n"
      "  7: 480x320\n"
      "  8: 640x480\n"
      "  9: 800x600\n"
      " 10: 1024x768\n"
      " 11: 1280x720\n"
      " 12: 1280x1024\n"
      " 13: 1600x1200\n"
      "Choose frame size (0-13): ",
      uwcCamFrameSize, true, true);
  uwc_udp_input("JPEG level compression (1-63): ", uwcCamJpegCompression, true,
                true);
  uwc_udp_input("Confirm?(y/n): ", confirm, false, true);

  if (strcmp(confirm, "y")) {
    uwc_udp_send("Camera setup aborted!\n");
    return;
  }

  uwc_event_nvs_write_framesize();
  uwc_event_nvs_write_jpeglevel();
  uwc_event_nvs_commit_auth();
  uwc_udp_send("Camera setup has been updated!\n");
}
