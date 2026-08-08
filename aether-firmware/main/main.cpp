#include "app_controller.hpp"

#include "esp_log.h"
#include "nvs_flash.h"

extern "C" void app_main(void) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  AppController app;
  ESP_ERROR_CHECK(app.init());
  app.start();

  // Phase 1: FreeRTOS tasks for UI tick, serial RX, and metric pump take over here.
}
