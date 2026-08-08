#pragma once

#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t aether_display_st7796_init(esp_lcd_panel_io_handle_t *io_out,
                                     esp_lcd_panel_handle_t *panel_out);
lv_display_t *aether_display_get_lv_display(void);

#ifdef __cplusplus
}
#endif
