#include "aether_webserial.h"

#include "driver/usb_serial_jtag.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string.h>

static const char *TAG = "aether_webserial";

static aether_webserial_msg_cb_t s_cb;
static void *s_user;
static TaskHandle_t s_task;
static bool s_host_linked;
static int64_t s_last_rx_us;

bool aether_webserial_host_linked(void) {
  if (!s_host_linked) return false;
  // Consider host gone if no frames for 3s.
  return (esp_timer_get_time() - s_last_rx_us) < 3000000;
}

esp_err_t aether_webserial_send(const char *line, size_t len) {
  if (!line || len == 0) return ESP_ERR_INVALID_ARG;
  int written = usb_serial_jtag_write_bytes(line, len, pdMS_TO_TICKS(100));
  return written == (int)len ? ESP_OK : ESP_FAIL;
}

static void handle_line(char *line) {
  aether_host_msg_t msg;
  esp_err_t err = aether_protocol_parse_line(line, &msg);
  if (err != ESP_OK) {
    return;
  }

  s_last_rx_us = esp_timer_get_time();
  if (msg.type == AETHER_MSG_HELLO_ACK || msg.type == AETHER_MSG_METRICS) {
    s_host_linked = true;
  }

  if (msg.type == AETHER_MSG_PING) {
    char pong[128];
    size_t n = aether_protocol_format_pong(pong, sizeof(pong), msg.ts);
    if (n) {
      (void)aether_webserial_send(pong, n);
    }
  }

  if (s_cb) {
    s_cb(&msg, s_user);
  }
}

static void webserial_task(void *arg) {
  (void)arg;
  char line[AETHER_PROTO_MAX_LINE];
  size_t idx = 0;
  uint8_t byte = 0;

  // Announce presence once transport is up.
  char hello[AETHER_PROTO_MAX_LINE];
  size_t n = aether_protocol_format_hello(hello, sizeof(hello));
  if (n) {
    (void)aether_webserial_send(hello, n);
  }

  while (true) {
    int r = usb_serial_jtag_read_bytes(&byte, 1, pdMS_TO_TICKS(50));
    if (r <= 0) {
      continue;
    }
    if (byte == '\r') {
      continue;
    }
    if (byte == '\n') {
      line[idx] = '\0';
      if (idx > 0) {
        handle_line(line);
      }
      idx = 0;
      continue;
    }
    if (idx + 1 < sizeof(line)) {
      line[idx++] = (char)byte;
    } else {
      // Overflow — drop line.
      idx = 0;
    }
  }
}

esp_err_t aether_webserial_init(aether_webserial_msg_cb_t cb, void *user) {
  s_cb = cb;
  s_user = user;
  s_host_linked = false;

  usb_serial_jtag_driver_config_t cfg = USB_SERIAL_JTAG_DRIVER_CONFIG_DEFAULT();
  cfg.rx_buffer_size = 2048;
  cfg.tx_buffer_size = 2048;
  esp_err_t err = usb_serial_jtag_driver_install(&cfg);
  if (err == ESP_ERR_INVALID_STATE) {
    // Already installed by console — continue.
    err = ESP_OK;
  }
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "usb_serial_jtag_driver_install failed: %s", esp_err_to_name(err));
    return err;
  }
  ESP_LOGI(TAG, "Web Serial transport ready (USB-Serial-JTAG)");
  return ESP_OK;
}

esp_err_t aether_webserial_start(void) {
  if (s_task) return ESP_OK;
  BaseType_t ok = xTaskCreate(webserial_task, "aether_ws", 4096, NULL, 5, &s_task);
  return ok == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}
