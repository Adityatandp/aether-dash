idf_component_register(
  SRCS "webserial/aether_webserial.c"
  INCLUDE_DIRS "include"
  REQUIRES aether_protocol driver esp_timer
)
