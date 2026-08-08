#include "app_controller.hpp"

#include "aether_board.h"
#include "aether_display.h"
#include "aether_input.h"
#include "aether_metrics_store.h"
#include "aether_protocol.h"
#include "aether_ui.h"
#include "aether_webserial.h"

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cmath>
#include <cstring>

static const char *TAG = "aether_app";

void AppController::on_transport_msg(const aether_host_msg_t *msg, void *user) {
  auto *self = static_cast<AppController *>(user);
  self->handle_msg(msg);
}

void AppController::on_input(const aether_input_event_t *evt, void *user) {
  auto *self = static_cast<AppController *>(user);
  self->handle_input(evt);
}

void AppController::handle_msg(const aether_host_msg_t *msg) {
  if (!msg) return;

  switch (msg->type) {
    case AETHER_MSG_METRICS:
      aether_metrics_store_update(&msg->metrics);
      aether_ui_set_metrics(&msg->metrics);
      if (!demo_mode_) {
        aether_ui_set_link_state(true, false);
      }
      break;
    case AETHER_MSG_HELLO_ACK:
      ESP_LOGI(TAG, "Host hello_ack received");
      if (!demo_mode_) {
        aether_ui_set_link_state(true, false);
      }
      break;
    case AETHER_MSG_CONFIG_SET:
      if (msg->has_brightness) {
        brightness_ = msg->brightness;
        (void)aether_display_set_brightness(brightness_);
      }
      break;
    default:
      break;
  }
}

void AppController::handle_input(const aether_input_event_t *evt) {
  if (!evt) return;

  if (evt->type == AETHER_INPUT_ENCODER_ROTATE) {
    int next = static_cast<int>(brightness_) + (evt->delta * 8);
    if (next < 10) next = 10;
    if (next > 255) next = 255;
    brightness_ = static_cast<uint8_t>(next);
    (void)aether_display_set_brightness(brightness_);

    char line[AETHER_PROTO_MAX_LINE];
    size_t n = aether_protocol_format_event(line, sizeof(line), "encoder_rotate", evt->delta);
    if (n) {
      (void)aether_webserial_send(line, n);
    }
  } else if (evt->type == AETHER_INPUT_ENCODER_BUTTON) {
    demo_mode_ = !demo_mode_;
    aether_ui_set_link_state(aether_webserial_host_linked(), demo_mode_);
    ESP_LOGI(TAG, "Demo mode %s", demo_mode_ ? "ON" : "OFF");

    char line[AETHER_PROTO_MAX_LINE];
    size_t n = aether_protocol_format_event(line, sizeof(line), "encoder_button", demo_mode_ ? 1 : 0);
    if (n) {
      (void)aether_webserial_send(line, n);
    }
  }
}

void AppController::ui_task(void *arg) {
  (void)arg;
  while (true) {
    aether_ui_tick_from_task();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void AppController::demo_task(void *arg) {
  auto *self = static_cast<AppController *>(arg);
  while (true) {
    if (self->demo_mode_ || !aether_webserial_host_linked()) {
      const int64_t t_us = esp_timer_get_time();
      const float t = static_cast<float>(t_us) / 1000000.0f;
      aether_metrics_t m = {};
      m.has_cpu = 1;
      m.has_ram = 1;
      m.has_gpu = 1;
      m.has_vram = 1;
      m.has_tokens = 1;
      m.has_watts = 1;
      m.has_gpu_temp = 1;
      m.has_label = 1;
      m.cpu_pct = 50.0f + 40.0f * sinf(t * 1.1f);
      m.ram_pct = 55.0f + 10.0f * sinf(t * 0.2f);
      m.gpu_pct = 50.0f + 40.0f * sinf(t * 0.8f + 1.0f);
      m.vram_pct = 60.0f + 15.0f * sinf(t * 0.15f + 2.0f);
      m.tokens_per_s = 18.0f + 12.0f * sinf(t * 0.7f);
      m.pkg_watts = 25.0f + 20.0f * sinf(t * 0.5f);
      m.gpu_temp_c = 55.0f + 12.0f * sinf(t * 0.25f);
      std::strncpy(m.label, "demo:local", sizeof(m.label) - 1);
      aether_metrics_store_update(&m);
      aether_ui_set_metrics(&m);
      aether_ui_set_link_state(aether_webserial_host_linked(), self->demo_mode_);
    } else {
      aether_ui_set_link_state(true, false);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

esp_err_t AppController::init() {
  const aether_board_info_t *board = aether_board_get();
  ESP_LOGI(TAG, "Board=%s layout=%d %ux%u", board->board_id, (int)board->layout_class,
           (unsigned)board->display_w, (unsigned)board->display_h);

  ESP_ERROR_CHECK(aether_metrics_store_init());

  aether_display_cfg_t disp_cfg = {
      .width = board->display_w,
      .height = board->display_h,
      .rotation = board->display_rotation,
  };
  ESP_ERROR_CHECK(aether_display_init(&disp_cfg));
  ESP_ERROR_CHECK(aether_display_set_brightness(brightness_));
  ESP_ERROR_CHECK(aether_ui_init());

  ESP_ERROR_CHECK(aether_input_init(on_input, this));
  ESP_ERROR_CHECK(aether_webserial_init(on_transport_msg, this));

  aether_ui_set_link_state(false, demo_mode_);
  return ESP_OK;
}

void AppController::start() {
  ESP_ERROR_CHECK(aether_webserial_start());
  ESP_ERROR_CHECK(aether_input_start());

  BaseType_t ok_ui = xTaskCreate(ui_task, "aether_ui", 4096, this, 4, nullptr);
  BaseType_t ok_demo = xTaskCreate(demo_task, "aether_demo", 4096, this, 3, nullptr);
  if (ok_ui != pdPASS || ok_demo != pdPASS) {
    ESP_LOGE(TAG, "Failed to start UI/demo tasks");
  }

  ESP_LOGI(TAG, "Phase 1 app started (Web Serial + LVGL AI/Dev + EC11)");
}
