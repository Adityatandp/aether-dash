#pragma once

#include "aether_protocol.h"
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t aether_ui_init(void);
void aether_ui_set_metrics(const aether_metrics_t *m);
void aether_ui_set_link_state(bool host_linked, bool demo_mode);
void aether_ui_tick_from_task(void);

#ifdef __cplusplus
}
#endif
