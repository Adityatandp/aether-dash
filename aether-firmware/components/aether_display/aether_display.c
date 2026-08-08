#include "aether_display.h"
#include "aether_display_priv.h"

#include "aether_board.h"
#include "pins.h"

#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"

static const char *TAG = "aether_display";
static lv_display_t *s_disp;
static uint8_t s_brightness = 180;

lv_display_t *aether_display_get_lv_display(void) { return s_disp; }

static esp_err_t backlight_init(void) {
  ledc_timer_config_t timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_8_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 5000,
      .clk_cfg = LEDC_AUTO_CLK,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&timer));

  ledc_channel_config_t ch = {
      .gpio_num = AETHER_PIN_LCD_BL,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = LEDC_CHANNEL_0,
      .timer_sel = LEDC_TIMER_0,
      .duty = s_brightness,
      .hpoint = 0,
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ch));
  return ESP_OK;
}

esp_err_t aether_display_set_brightness(uint8_t level_0_255) {
  s_brightness = level_0_255;
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, s_brightness));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
  return ESP_OK;
}

void aether_display_lvgl_lock(void) { lvgl_port_lock(0); }

void aether_display_lvgl_unlock(void) { lvgl_port_unlock(); }

esp_err_t aether_display_init(const aether_display_cfg_t *cfg) {
  (void)cfg;
  const aether_board_info_t *board = aether_board_get();

  ESP_ERROR_CHECK(backlight_init());

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_handle_t panel_handle = NULL;
  ESP_ERROR_CHECK(aether_display_st7796_init(&io_handle, &panel_handle));

  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

  const lvgl_port_display_cfg_t disp_cfg = {
      .io_handle = io_handle,
      .panel_handle = panel_handle,
      .buffer_size = (uint32_t)board->display_w * 40,
      .double_buffer = true,
      .hres = board->display_w,
      .vres = board->display_h,
      .monochrome = false,
#if LVGL_VERSION_MAJOR >= 9
      .color_format = LV_COLOR_FORMAT_RGB565,
#endif
      .rotation =
          {
              .swap_xy = true,
              .mirror_x = true,
              .mirror_y = false,
          },
      .flags =
          {
              .buff_dma = true,
              .swap_bytes = true,
          },
  };

  s_disp = lvgl_port_add_disp(&disp_cfg);
  if (!s_disp) {
    ESP_LOGE(TAG, "lvgl_port_add_disp failed");
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Display + LVGL port initialized");
  return ESP_OK;
}
