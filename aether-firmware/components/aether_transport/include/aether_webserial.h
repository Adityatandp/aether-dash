#pragma once

#include "aether_protocol.h"
#include "esp_err.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*aether_webserial_msg_cb_t)(const aether_host_msg_t *msg, void *user);

esp_err_t aether_webserial_init(aether_webserial_msg_cb_t cb, void *user);
esp_err_t aether_webserial_start(void);
esp_err_t aether_webserial_send(const char *line, size_t len);
bool aether_webserial_host_linked(void);

#ifdef __cplusplus
}
#endif
