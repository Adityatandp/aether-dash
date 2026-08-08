#include "aether_protocol.h"

#include "aether_board.h"

#include <stdio.h>
#include <string.h>

/* Phase 1 note: replace lightweight string checks with cJSON / json_parser. */

esp_err_t aether_protocol_parse_line(const char *line, aether_host_msg_t *out) {
  if (!line || !out) {
    return ESP_ERR_INVALID_ARG;
  }
  memset(out, 0, sizeof(*out));

  if (strstr(line, "\"type\":\"metrics\"") || strstr(line, "\"type\": \"metrics\"")) {
    out->type = AETHER_MSG_METRICS;
    /* Stub: real parsing lands in Phase 1 week 2. */
    out->metrics.has_cpu = 1;
    out->metrics.cpu_pct = 0;
    return ESP_OK;
  }
  if (strstr(line, "\"type\":\"hello_ack\"") || strstr(line, "\"type\": \"hello_ack\"")) {
    out->type = AETHER_MSG_HELLO_ACK;
    return ESP_OK;
  }
  if (strstr(line, "\"type\":\"ping\"") || strstr(line, "\"type\": \"ping\"")) {
    out->type = AETHER_MSG_PING;
    return ESP_OK;
  }
  if (strstr(line, "\"type\":\"config_set\"") || strstr(line, "\"type\": \"config_set\"")) {
    out->type = AETHER_MSG_CONFIG_SET;
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
