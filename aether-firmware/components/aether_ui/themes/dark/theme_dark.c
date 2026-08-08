#include "aether_theme.h"

#include "lvgl.h"

void aether_theme_dark_apply(void) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x070B14), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
}
