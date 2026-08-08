#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AETHER_PROTO_VERSION 1
#define AETHER_PROTO_MAX_LINE 1024

typedef struct {
  float cpu_pct;
  float ram_pct;
  float gpu_pct;
  float vram_pct;
  float tokens_per_s;
  float pkg_watts;
  float gpu_temp_c;
  char label[65];
  uint8_t has_cpu : 1;
  uint8_t has_ram : 1;
  uint8_t has_gpu : 1;
  uint8_t has_vram : 1;
  uint8_t has_tokens : 1;
  uint8_t has_watts : 1;
  uint8_t has_gpu_temp : 1;
} aether_metrics_t;

typedef enum {
  AETHER_MSG_UNKNOWN = 0,
  AETHER_MSG_HELLO_ACK,
  AETHER_MSG_METRICS,
  AETHER_MSG_CONFIG_SET,
  AETHER_MSG_PING,
} aether_msg_type_t;

typedef struct {
  aether_msg_type_t type;
  int64_t ts;
  aether_metrics_t metrics;
  uint8_t brightness; /* for config_set, 0=unset */
} aether_host_msg_t;

esp_err_t aether_protocol_parse_line(const char *line, aether_host_msg_t *out);
size_t aether_protocol_format_hello(char *buf, size_t buflen);
size_t aether_protocol_format_pong(char *buf, size_t buflen, int64_t ts);

#ifdef __cplusplus
}
#endif
