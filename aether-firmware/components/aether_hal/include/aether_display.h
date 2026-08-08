#pragma once

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t rotation;
} aether_display_cfg_t;

esp_err_t aether_display_init(const aether_display_cfg_t *cfg);
esp_err_t aether_display_set_brightness(uint8_t level_0_255);
void aether_display_lvgl_lock(void);
void aether_display_lvgl_unlock(void);

#ifdef __cplusplus
}
#endif
