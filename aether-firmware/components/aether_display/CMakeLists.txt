idf_component_register(
  SRCS
    "aether_display.c"
    "st7796/aether_display_st7796.c"
  INCLUDE_DIRS
    "include"
    "."
  REQUIRES
    aether_hal
    driver
    esp_lcd
    esp_lvgl_port
    esp_lcd_st7796
)

# Board pins.h (selected via -D AETHER_BOARD=...)
if(NOT DEFINED AETHER_BOARD)
  set(AETHER_BOARD "dash-s35")
endif()
target_include_directories(${COMPONENT_LIB} PRIVATE "${CMAKE_SOURCE_DIR}/boards/${AETHER_BOARD}")
