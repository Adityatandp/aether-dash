#pragma once

#include "aether_protocol.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t aether_metrics_store_init(void);
void aether_metrics_store_update(const aether_metrics_t *m);
void aether_metrics_store_get(aether_metrics_t *out);

#ifdef __cplusplus
}
#endif
