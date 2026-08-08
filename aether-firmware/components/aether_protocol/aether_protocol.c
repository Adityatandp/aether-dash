#include "aether_protocol.h"

#include "aether_board.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool json_contains_type(const char *line, const char *type_name) {
  char pattern_a[64];
  char pattern_b[64];
  snprintf(pattern_a, sizeof(pattern_a), "\"type\":\"%s\"", type_name);
  snprintf(pattern_b, sizeof(pattern_b), "\"type\": \"%s\"", type_name);
  return strstr(line, pattern_a) != NULL || strstr(line, pattern_b) != NULL;
}

static const char *find_key(const char *json, const char *key) {
  char pattern[80];
  snprintf(pattern, sizeof(pattern), "\"%s\"", key);
  const char *p = strstr(json, pattern);
  if (!p) return NULL;
  p += strlen(pattern);
  while (*p && (isspace((unsigned char)*p) || *p == ':')) {
    ++p;
  }
  return *p ? p : NULL;
}

static bool json_get_number(const char *json, const char *key, float *out) {
  const char *p = find_key(json, key);
  if (!p) return false;
  char *end = NULL;
  float v = strtof(p, &end);
  if (end == p) return false;
  *out = v;
  return true;
}

static bool json_get_int(const char *json, const char *key, long *out) {
  const char *p = find_key(json, key);
  if (!p) return false;
  char *end = NULL;
  long v = strtol(p, &end, 10);
  if (end == p) return false;
  *out = v;
  return true;
}

static bool json_get_string(const char *json, const char *key, char *out, size_t outlen) {
  const char *p = find_key(json, key);
  if (!p || *p != '"') return false;
  ++p;
  size_t i = 0;
  while (*p && *p != '"' && i + 1 < outlen) {
    if (*p == '\\' && p[1]) {
      ++p;
    }
    out[i++] = *p++;
  }
  out[i] = '\0';
  return i > 0 || (*p == '"');
}

static void parse_metrics_payload(const char *line, aether_metrics_t *m) {
  float v = 0;
  if (json_get_number(line, "cpu_pct", &v)) {
    m->cpu_pct = v;
    m->has_cpu = 1;
  }
  if (json_get_number(line, "ram_pct", &v)) {
    m->ram_pct = v;
    m->has_ram = 1;
  }
  if (json_get_number(line, "gpu_pct", &v)) {
    m->gpu_pct = v;
    m->has_gpu = 1;
  }
  if (json_get_number(line, "vram_pct", &v)) {
    m->vram_pct = v;
    m->has_vram = 1;
  }
  if (json_get_number(line, "tokens_per_s", &v)) {
    m->tokens_per_s = v;
    m->has_tokens = 1;
  }
  if (json_get_number(line, "pkg_watts", &v)) {
    m->pkg_watts = v;
    m->has_watts = 1;
  }
  if (json_get_number(line, "gpu_temp_c", &v)) {
    m->gpu_temp_c = v;
    m->has_gpu_temp = 1;
  }
  if (json_get_string(line, "label", m->label, sizeof(m->label))) {
    m->has_label = 1;
  }
}

esp_err_t aether_protocol_parse_line(const char *line, aether_host_msg_t *out) {
  if (!line || !out) {
    return ESP_ERR_INVALID_ARG;
  }
  memset(out, 0, sizeof(*out));

  long ts = 0;
  if (json_get_int(line, "ts", &ts)) {
    out->ts = ts;
  }

  if (json_contains_type(line, "metrics")) {
    out->type = AETHER_MSG_METRICS;
    parse_metrics_payload(line, &out->metrics);
    return ESP_OK;
  }
  if (json_contains_type(line, "hello_ack")) {
    out->type = AETHER_MSG_HELLO_ACK;
    return ESP_OK;
  }
  if (json_contains_type(line, "ping")) {
    out->type = AETHER_MSG_PING;
    return ESP_OK;
  }
  if (json_contains_type(line, "config_set")) {
    out->type = AETHER_MSG_CONFIG_SET;
    long brightness = 0;
    if (json_get_int(line, "brightness", &brightness) && brightness > 0 && brightness <= 255) {
      out->brightness = (uint8_t)brightness;
      out->has_brightness = 1;
    }
    (void)json_get_string(line, "theme", out->theme, sizeof(out->theme));
    (void)json_get_string(line, "mode", out->mode, sizeof(out->mode));
    return ESP_OK;
  }

  out->type = AETHER_MSG_UNKNOWN;
  return ESP_ERR_NOT_FOUND;
}

size_t aether_protocol_format_hello(char *buf, size_t buflen) {
  const aether_board_info_t *b = aether_board_get();
  const char *layout = "M";
  if (b->layout_class == AETHER_LAYOUT_S) layout = "S";
  if (b->layout_class == AETHER_LAYOUT_L) layout = "L";

  int n = snprintf(
      buf, buflen,
      "{\"v\":1,\"type\":\"hello\",\"ts\":0,\"payload\":{"
      "\"board_id\":\"%s\",\"fw\":\"0.1.0-dev\",\"protocol\":\"1.0\","
      "\"layout_class\":\"%s\",\"caps\":{\"webserial\":%s,\"webusb\":%s,"
      "\"wifi_mqtt\":%s,\"encoder\":%s,\"touch\":%s},"
      "\"display\":{\"w\":%u,\"h\":%u,\"rotation\":%u}}}\n",
      b->board_id, layout, b->has_webserial ? "true" : "false",
      b->has_webusb ? "true" : "false", b->has_wifi_mqtt ? "true" : "false",
      b->has_encoder ? "true" : "false", b->has_touch ? "true" : "false",
      (unsigned)b->display_w, (unsigned)b->display_h,
      (unsigned)b->display_rotation);
  if (n < 0) return 0;
  return (size_t)n >= buflen ? 0 : (size_t)n;
}

size_t aether_protocol_format_pong(char *buf, size_t buflen, int64_t ts) {
  int n = snprintf(buf, buflen,
                   "{\"v\":1,\"type\":\"pong\",\"ts\":%lld,\"payload\":{}}\n",
                   (long long)ts);
  if (n < 0) return 0;
  return (size_t)n >= buflen ? 0 : (size_t)n;
}

size_t aether_protocol_format_event(char *buf, size_t buflen, const char *name, int delta) {
  int n = snprintf(buf, buflen,
                   "{\"v\":1,\"type\":\"event\",\"ts\":0,\"payload\":{"
                   "\"name\":\"%s\",\"delta\":%d}}\n",
                   name ? name : "unknown", delta);
  if (n < 0) return 0;
  return (size_t)n >= buflen ? 0 : (size_t)n;
}
