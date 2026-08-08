idf_component_register(
  SRCS "main.cpp" "app_controller.cpp"
  INCLUDE_DIRS "."
  REQUIRES
    aether_hal
    aether_protocol
    aether_metrics
    aether_display
    aether_ui
    aether_input
    aether_transport
    nvs_flash
)
