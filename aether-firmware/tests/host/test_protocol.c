#include "aether_protocol.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_metrics_parse(void) {
  const char *line =
      "{\"v\":1,\"type\":\"metrics\",\"ts\":123,"
      "\"payload\":{\"cpu_pct\":42.5,\"ram_pct\":61,\"gpu_pct\":33,"
      "\"vram_pct\":70,\"tokens_per_s\":28.4,\"pkg_watts\":35.2,"
      "\"gpu_temp_c\":64,\"label\":\"ollama:llama\"}}";

  aether_host_msg_t msg;
  assert(aether_protocol_parse_line(line, &msg) == ESP_OK);
  assert(msg.type == AETHER_MSG_METRICS);
  assert(msg.ts == 123);
  assert(msg.metrics.has_cpu);
  assert(msg.metrics.cpu_pct > 42.4f && msg.metrics.cpu_pct < 42.6f);
  assert(msg.metrics.has_tokens);
  assert(msg.metrics.tokens_per_s > 28.3f && msg.metrics.tokens_per_s < 28.5f);
  assert(msg.metrics.has_label);
  assert(strcmp(msg.metrics.label, "ollama:llama") == 0);
  puts("OK metrics_parse");
}

static void test_ping_pong(void) {
  aether_host_msg_t msg;
  assert(aether_protocol_parse_line("{\"v\":1,\"type\":\"ping\",\"ts\":9,\"payload\":{}}", &msg) ==
         ESP_OK);
  assert(msg.type == AETHER_MSG_PING);

  char buf[128];
  size_t n = aether_protocol_format_pong(buf, sizeof(buf), 9);
  assert(n > 0);
  assert(strstr(buf, "\"type\":\"pong\"") != NULL);
  puts("OK ping_pong");
}

static void test_hello(void) {
  char buf[AETHER_PROTO_MAX_LINE];
  size_t n = aether_protocol_format_hello(buf, sizeof(buf));
  assert(n > 0);
  assert(strstr(buf, "\"type\":\"hello\"") != NULL);
  assert(strstr(buf, "dash-s35") != NULL || strstr(buf, "board_id") != NULL);
  puts("OK hello");
}

static void test_config_set(void) {
  aether_host_msg_t msg;
  const char *line =
      "{\"v\":1,\"type\":\"config_set\",\"ts\":1,\"payload\":{\"brightness\":200,"
      "\"theme\":\"dark\",\"mode\":\"ai_dev\"}}";
  assert(aether_protocol_parse_line(line, &msg) == ESP_OK);
  assert(msg.type == AETHER_MSG_CONFIG_SET);
  assert(msg.has_brightness);
  assert(msg.brightness == 200);
  assert(strcmp(msg.theme, "dark") == 0);
  puts("OK config_set");
}

int main(void) {
  test_metrics_parse();
  test_ping_pong();
  test_hello();
  test_config_set();
  puts("All host protocol tests passed");
  return 0;
}
