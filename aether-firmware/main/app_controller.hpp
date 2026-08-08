#pragma once

#include "aether_input.h"
#include "aether_protocol.h"

#include "esp_err.h"

class AppController {
 public:
  esp_err_t init();
  void start();

 private:
  static void on_transport_msg(const aether_host_msg_t *msg, void *user);
  static void on_input(const aether_input_event_t *evt, void *user);
  static void ui_task(void *arg);
  static void demo_task(void *arg);

  void handle_msg(const aether_host_msg_t *msg);
  void handle_input(const aether_input_event_t *evt);

  bool demo_mode_ = true;
  uint8_t brightness_ = 180;
};
