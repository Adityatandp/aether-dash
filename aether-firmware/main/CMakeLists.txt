idf_component_register(
  SRCS "main.cpp" "app_controller.cpp"
  INCLUDE_DIRS "."
  REQUIRES aether_hal aether_protocol aether_metrics
)
