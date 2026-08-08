#include "aether_board.h"

#ifndef AETHER_BOARD_ID
#define AETHER_BOARD_ID "dash-s35"
#endif

/* Phase 0/1: compile-time defaults for launch SKU.
 * Later: generate from board.json or include board-specific overrides.
 */
static const aether_board_info_t s_board = {
    .board_id = AETHER_BOARD_ID,
    .mcu = "esp32s3",
    .display_w = 480,
    .display_h = 320,
    .display_rotation = 90,
    .layout_class = AETHER_LAYOUT_M,
    .has_encoder = true,
    .has_touch = false,
    .has_webserial = true,
    .has_webusb = false,
    .has_wifi_mqtt = false,
};

const aether_board_info_t *aether_board_get(void) { return &s_board; }
