#include "aether_input.h"

#include "pins.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

static const char *TAG = "aether_encoder";

static aether_input_cb_t s_cb;
static void *s_user;
static QueueHandle_t s_q;
static int64_t s_last_btn_us;

typedef struct {
  aether_input_event_t evt;
} enc_msg_t;

static void IRAM_ATTR encoder_isr(void *arg) {
  (void)arg;
  static uint8_t last_state = 0;
  uint8_t a = gpio_get_level(AETHER_PIN_ENC_A);
  uint8_t b = gpio_get_level(AETHER_PIN_ENC_B);
  uint8_t state = (a << 1) | b;

  // Simple quadrature: detect transitions on A.
  int8_t delta = 0;
  if ((last_state == 0b00 && state == 0b10) || (last_state == 0b10 && state == 0b11) ||
      (last_state == 0b11 && state == 0b01) || (last_state == 0b01 && state == 0b00)) {
    delta = 1;
  } else if ((last_state == 0b00 && state == 0b01) || (last_state == 0b01 && state == 0b11) ||
             (last_state == 0b11 && state == 0b10) || (last_state == 0b10 && state == 0b00)) {
    delta = -1;
  }
  last_state = state;

  if (delta != 0 && s_q) {
    enc_msg_t msg = {.evt = {.type = AETHER_INPUT_ENCODER_ROTATE, .delta = delta, .pressed = 0}};
    BaseType_t hp = pdFALSE;
    xQueueSendFromISR(s_q, &msg, &hp);
    if (hp) portYIELD_FROM_ISR();
  }
}

static void IRAM_ATTR button_isr(void *arg) {
  (void)arg;
  int64_t now = esp_timer_get_time();
  if (now - s_last_btn_us < 250000) {
    return; // debounce 250ms
  }
  s_last_btn_us = now;
  if (!s_q) return;
  enc_msg_t msg = {.evt = {.type = AETHER_INPUT_ENCODER_BUTTON, .delta = 0, .pressed = 1}};
  BaseType_t hp = pdFALSE;
  xQueueSendFromISR(s_q, &msg, &hp);
  if (hp) portYIELD_FROM_ISR();
}

static void input_task(void *arg) {
  (void)arg;
  enc_msg_t msg;
  while (true) {
    if (xQueueReceive(s_q, &msg, portMAX_DELAY) == pdTRUE) {
      if (s_cb) {
        s_cb(&msg.evt, s_user);
      }
    }
  }
}

esp_err_t aether_input_init(aether_input_cb_t cb, void *user) {
  s_cb = cb;
  s_user = user;
  s_q = xQueueCreate(16, sizeof(enc_msg_t));
  if (!s_q) return ESP_ERR_NO_MEM;

  gpio_config_t io = {
      .pin_bit_mask = (1ULL << AETHER_PIN_ENC_A) | (1ULL << AETHER_PIN_ENC_B) | (1ULL << AETHER_PIN_ENC_BTN),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  ESP_ERROR_CHECK(gpio_config(&io));

  ESP_ERROR_CHECK(gpio_set_intr_type(AETHER_PIN_ENC_A, GPIO_INTR_ANYEDGE));
  ESP_ERROR_CHECK(gpio_set_intr_type(AETHER_PIN_ENC_B, GPIO_INTR_ANYEDGE));
  ESP_ERROR_CHECK(gpio_set_intr_type(AETHER_PIN_ENC_BTN, GPIO_INTR_NEGEDGE));
  ESP_ERROR_CHECK(gpio_install_isr_service(0));
  ESP_ERROR_CHECK(gpio_isr_handler_add(AETHER_PIN_ENC_A, encoder_isr, NULL));
  ESP_ERROR_CHECK(gpio_isr_handler_add(AETHER_PIN_ENC_B, encoder_isr, NULL));
  ESP_ERROR_CHECK(gpio_isr_handler_add(AETHER_PIN_ENC_BTN, button_isr, NULL));

  ESP_LOGI(TAG, "EC11 encoder ready");
  return ESP_OK;
}

esp_err_t aether_input_start(void) {
  BaseType_t ok = xTaskCreate(input_task, "aether_enc", 3072, NULL, 4, NULL);
  return ok == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}
