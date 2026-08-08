#pragma once

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  AETHER_INPUT_NONE = 0,
  AETHER_INPUT_ENCODER_ROTATE,
  AETHER_INPUT_ENCODER_BUTTON,
} aether_input_type_t;

typedef struct {
  aether_input_type_t type;
  int16_t delta; /* for rotate */
  uint8_t pressed; /* 0/1 for button */
} aether_input_event_t;

typedef void (*aether_input_cb_t)(const aether_input_event_t *evt, void *user);

esp_err_t aether_input_init(aether_input_cb_t cb, void *user);
esp_err_t aether_input_start(void);

#ifdef __cplusplus
}
#endif
