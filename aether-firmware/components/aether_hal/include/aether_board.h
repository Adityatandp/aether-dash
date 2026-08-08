#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  AETHER_LAYOUT_S = 0,
  AETHER_LAYOUT_M = 1,
  AETHER_LAYOUT_L = 2,
} aether_layout_class_t;

typedef struct {
  const char *board_id;
  const char *mcu;
  uint16_t display_w;
  uint16_t display_h;
  uint16_t display_rotation;
  aether_layout_class_t layout_class;
  bool has_encoder;
  bool has_touch;
  bool has_webserial;
  bool has_webusb;
  bool has_wifi_mqtt;
} aether_board_info_t;

/** Populate board info from compile-time profile / board.json-derived headers. */
const aether_board_info_t *aether_board_get(void);

#ifdef __cplusplus
}
#endif
