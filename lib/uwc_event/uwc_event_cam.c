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

  char compressedFb[16384];

  camera_fb_t *fb = esp_camera_fb_get();

  z_stream deflstream;

  deflstream.zalloc = Z_NULL;
  deflstream.zfree = Z_NULL;
  deflstream.opaque = Z_NULL;

  // Prepare compression
  deflstream.avail_in = (uInt)fb->len;
  deflstream.next_in = (Bytef *)fb->buf;
  deflstream.avail_out = (uInt)sizeof(compressedFb);
  deflstream.next_out = (Bytef *)compressedFb;

  // Actual compression
  deflateInit(&deflstream, Z_DEFAULT_COMPRESSION);
  deflate(&deflstream, Z_FINISH);
  deflateEnd(&deflstream);

  ESP_LOGI(uwc_tag_event, "Actual Size: %u", fb->len);
  esp_camera_fb_return(fb);

  char *from = &compressedFb[0];
  size_t toSend = (size_t)deflstream.total_out;
  ESP_LOGI(uwc_tag_event, "Compressed:Actual = %u:%u",
           (size_t)deflstream.total_out,
           (size_t)deflstream.total_in);  // Just for debugging

  for (int i = 0; toSend > 0; i++) {
    size_t sendSize = toSend > UDP_BUF_SIZE ? UDP_BUF_SIZE : toSend;
    while (uwc_udp_send_raw(from, sendSize) < 0) {
      ESP_LOGW(uwc_tag_event, "Retry in itteration: %i", i);
    }
    toSend -= sendSize;
    from += sendSize;
  }
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

  // Initializing for compression (zlib)
  z_stream deflstream;
  deflstream.zalloc = Z_NULL;
  deflstream.zfree = Z_NULL;
  deflstream.opaque = Z_NULL;

  uwcEventErr = uwc_cam_open();
  if (uwcEventErr) {
    ESP_LOGE(uwc_tag_event, "Camera get frame buffer failed!");
    return;
  }
  ESP_LOGI(uwc_tag_event, "Camera capture test OK with size: %u",
           uwcCamFb->len);
  // Prepare compression
  char compressedFb[16384];
  deflstream.avail_in = (uInt)uwcCamFb->len;
  deflstream.next_in = (Bytef *)uwcCamFb->buf;
  deflstream.avail_out = (uInt)sizeof(compressedFb);
  deflstream.next_out = (Bytef *)compressedFb;

  // Actual compression
  deflateInit(&deflstream, Z_BEST_COMPRESSION);
  deflate(&deflstream, Z_FINISH);
  deflateEnd(&deflstream);

  ESP_LOGI(uwc_tag_event, "Compressed:Actual = %u:%u",
           (size_t)deflstream.total_out,
           (size_t)deflstream.total_in);  // Just for debugging
  uwc_cam_close();

  for (;;) {  // streaming video...
    // z_stream deflstream;  // create struct

    // // I don't know what these do I just copied paste
    // deflstream.zalloc = Z_NULL;
    // deflstream.zfree = Z_NULL;
    // deflstream.opaque = Z_NULL;

    // // Prepare compression
    // camera_fb_t *fb =
    //     esp_camera_fb_get();  // getting current JPEG frame buffer location
    // deflstream.avail_in = (uInt)fb->len;    // Set framebuffer length
    // deflstream.next_in = (Bytef *)fb->buf;  // Set location of framebuffer
    // esp_camera_fb_return(fb);
    // memset(&compressedFb[0], 0, 16384);
    // deflstream.avail_out =
    //     (uInt)sizeof(compressedFb);  // alocated memory size for output
    // deflstream.next_out = (Bytef *)compressedFb;  // the output

    // // Actual compression
    // deflateInit(&deflstream, Z_DEFAULT_COMPRESSION);
    // deflate(&deflstream, Z_FINISH);  // What is Z_FINISH?
    // deflateEnd(&deflstream);         // Just doing it because its ending.

    // ESP_LOGI(uwc_tag_event, "Compressed:Actual = %u:%u",
    //          (size_t)deflstream.total_out,
    //          (size_t)deflstream.total_in);  // Just for debugging

    // char *from = &compressedFb[0];  // This var is just copy location  of
    // size_t toSend =
    //     (size_t)
    //         deflstream.total_out;  // This just track remaining bytes to be
    //         sent

    camera_fb_t *fb = esp_camera_fb_get();
    register uint8_t *from =
        fb->buf;  // This var is just copy location  of output
    register size_t toSend = fb->len;

    for (register int i = 0; toSend > 0; i++) {  // Sending packet per packet...
      register size_t sendSize = toSend > UDP_BUF_SIZE ? UDP_BUF_SIZE : toSend;
      while (uwc_udp_send_raw(from, sendSize) < 0) {
        ESP_LOGW(uwc_tag_event, "Retry in itteration: %i", i);
      }
      toSend -= sendSize;
      from += sendSize;
    }
    esp_camera_fb_return(fb);
  }
}
