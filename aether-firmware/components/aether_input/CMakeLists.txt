idf_component_register(
  SRCS "aether_encoder.c"
  INCLUDE_DIRS "."
  REQUIRES aether_hal driver esp_timer
)

if(NOT DEFINED AETHER_BOARD)
  set(AETHER_BOARD "dash-s35")
endif()
target_include_directories(${COMPONENT_LIB} PRIVATE "${CMAKE_SOURCE_DIR}/boards/${AETHER_BOARD}")
