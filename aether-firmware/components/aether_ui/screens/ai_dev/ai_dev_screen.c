#include "aether_ui.h"

#include "aether_display.h"
#include "aether_theme.h"

#include "esp_log.h"
#include "lvgl.h"

#include <stdio.h>

static const char *TAG = "aether_ui_ai";

typedef struct {
  lv_obj_t *title;
  lv_obj_t *status;
  lv_obj_t *tokens_value;
  lv_obj_t *cpu_arc;
  lv_obj_t *gpu_arc;
  lv_obj_t *cpu_label;
  lv_obj_t *gpu_label;
  lv_obj_t *ram_label;
  lv_obj_t *vram_label;
  lv_obj_t *watts_label;
  lv_obj_t *temp_label;
  lv_obj_t *model_label;
} ai_dev_ui_t;

static ai_dev_ui_t s_ui;
static aether_metrics_t s_metrics;
static bool s_host_linked;
static bool s_demo_mode = true;

static lv_obj_t *make_label(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y, uint32_t color) {
  lv_obj_t *lbl = lv_label_create(parent);
  lv_label_set_text(lbl, text);
  lv_obj_set_style_text_color(lbl, lv_color_hex(color), 0);
  lv_obj_set_pos(lbl, x, y);
  return lbl;
}

static lv_obj_t *make_arc(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, uint32_t color) {
  lv_obj_t *arc = lv_arc_create(parent);
  lv_obj_set_size(arc, 140, 140);
  lv_obj_set_pos(arc, x, y);
  lv_arc_set_rotation(arc, 135);
  lv_arc_set_bg_angles(arc, 0, 270);
  lv_arc_set_range(arc, 0, 100);
  lv_arc_set_value(arc, 0);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_arc_color(arc, lv_color_hex(0x1E293B), LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 12, LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc, lv_color_hex(color), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc, 12, LV_PART_INDICATOR);
  return arc;
}

esp_err_t aether_ui_init(void) {
  aether_display_lvgl_lock();

  aether_theme_dark_apply();
  lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  s_ui.title = make_label(scr, "AETHER  ·  AI / DEV", 16, 10, 0x67E8F9);
  s_ui.status = make_label(scr, "DEMO MODE", 360, 12, 0xFBBF24);
  s_ui.tokens_value = make_label(scr, "0.0 tok/s", 16, 36, 0xF8FAFC);
  lv_obj_set_style_text_font(s_ui.tokens_value, &lv_font_montserrat_20, 0);

  s_ui.cpu_arc = make_arc(scr, 40, 90, 0x22D3EE);
  s_ui.gpu_arc = make_arc(scr, 300, 90, 0x3B82F6);
  s_ui.cpu_label = make_label(scr, "CPU 0%", 78, 148, 0xE2E8F0);
  s_ui.gpu_label = make_label(scr, "GPU 0%", 338, 148, 0xE2E8F0);

  s_ui.ram_label = make_label(scr, "RAM --%", 16, 250, 0x94A3B8);
  s_ui.vram_label = make_label(scr, "VRAM --%", 130, 250, 0x94A3B8);
  s_ui.watts_label = make_label(scr, "PKG -- W", 250, 250, 0x94A3B8);
  s_ui.temp_label = make_label(scr, "GPU -- C", 370, 250, 0x94A3B8);
  s_ui.model_label = make_label(scr, "label: --", 16, 286, 0x64748B);

  make_label(scr, "ENC: brightness  |  PUSH: demo/live", 16, 300, 0x475569);

  aether_display_lvgl_unlock();
  ESP_LOGI(TAG, "AI/Dev dual-gauge screen ready");
  return ESP_OK;
}

void aether_ui_set_metrics(const aether_metrics_t *m) {
  if (!m) return;
  s_metrics = *m;
}

void aether_ui_set_link_state(bool host_linked, bool demo_mode) {
  s_host_linked = host_linked;
  s_demo_mode = demo_mode;
}

void aether_ui_tick_from_task(void) {
  char buf[48];

  aether_display_lvgl_lock();

  float cpu = s_metrics.has_cpu ? s_metrics.cpu_pct : 0;
  float gpu = s_metrics.has_gpu ? s_metrics.gpu_pct : 0;
  float tokens = s_metrics.has_tokens ? s_metrics.tokens_per_s : 0;

  lv_arc_set_value(s_ui.cpu_arc, (int16_t)cpu);
  lv_arc_set_value(s_ui.gpu_arc, (int16_t)gpu);

  snprintf(buf, sizeof(buf), "CPU %.0f%%", cpu);
  lv_label_set_text(s_ui.cpu_label, buf);
  snprintf(buf, sizeof(buf), "GPU %.0f%%", gpu);
  lv_label_set_text(s_ui.gpu_label, buf);
  snprintf(buf, sizeof(buf), "%.1f tok/s", tokens);
  lv_label_set_text(s_ui.tokens_value, buf);

  if (!s_metrics.has_ram) snprintf(buf, sizeof(buf), "RAM --%%");
  else snprintf(buf, sizeof(buf), "RAM %.0f%%", s_metrics.ram_pct);
  lv_label_set_text(s_ui.ram_label, buf);

  if (!s_metrics.has_vram) snprintf(buf, sizeof(buf), "VRAM --%%");
  else snprintf(buf, sizeof(buf), "VRAM %.0f%%", s_metrics.vram_pct);
  lv_label_set_text(s_ui.vram_label, buf);

  if (!s_metrics.has_watts) snprintf(buf, sizeof(buf), "PKG -- W");
  else snprintf(buf, sizeof(buf), "PKG %.0f W", s_metrics.pkg_watts);
  lv_label_set_text(s_ui.watts_label, buf);

  if (!s_metrics.has_gpu_temp) snprintf(buf, sizeof(buf), "GPU -- C");
  else snprintf(buf, sizeof(buf), "GPU %.0f C", s_metrics.gpu_temp_c);
  lv_label_set_text(s_ui.temp_label, buf);

  if (s_metrics.has_label) {
    snprintf(buf, sizeof(buf), "label: %s", s_metrics.label);
  } else {
    snprintf(buf, sizeof(buf), "label: --");
  }
  lv_label_set_text(s_ui.model_label, buf);

  if (s_demo_mode) {
    lv_label_set_text(s_ui.status, "DEMO MODE");
    lv_obj_set_style_text_color(s_ui.status, lv_color_hex(0xFBBF24), 0);
  } else if (s_host_linked) {
    lv_label_set_text(s_ui.status, "LIVE USB");
    lv_obj_set_style_text_color(s_ui.status, lv_color_hex(0x34D399), 0);
  } else {
    lv_label_set_text(s_ui.status, "WAITING");
    lv_obj_set_style_text_color(s_ui.status, lv_color_hex(0x94A3B8), 0);
  }

  aether_display_lvgl_unlock();
}
