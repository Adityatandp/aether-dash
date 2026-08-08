#pragma once

#include "esp_err.h"

class AppController {
 public:
  esp_err_t init();
  void start();
};
