#include "app_controller.hpp"

#include "aether_board.h"
#include "aether_metrics_store.h"
#include "aether_protocol.h"

#include "esp_log.h"

#include <cstdio>

static const char *TAG = "aether_app";

esp_err_t AppController::init() {
  const aether_board_info_t *board = aether_board_get();
  ESP_LOGI(TAG, "Board=%s layout=%d %ux%u", board->board_id,
           (int)board->layout_class, (unsigned)board->display_w,
           (unsigned)board->display_h);

  esp_err_t err = aether_metrics_store_init();
  if (err != ESP_OK) {
    return err;
  }

  // Phase 1: init display HAL, LVGL UI, encoder, and Web Serial transport here.
  ESP_LOGW(TAG, "Display/UI/transport bring-up pending Phase 1 implementation");
  return ESP_OK;
}

void AppController::start() {
  char hello[AETHER_PROTO_MAX_LINE];
  size_t n = aether_protocol_format_hello(hello, sizeof(hello));
  if (n > 0) {
    // Until CDC transport lands, emit hello to console for bring-up visibility.
    fputs(hello, stdout);
    fflush(stdout);
  }
  ESP_LOGI(TAG, "App controller started (protocol hello emitted)");
}
