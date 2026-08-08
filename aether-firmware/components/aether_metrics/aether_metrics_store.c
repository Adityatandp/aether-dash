#include "aether_metrics_store.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include <string.h>

static aether_metrics_t s_metrics;
static SemaphoreHandle_t s_lock;

esp_err_t aether_metrics_store_init(void) {
  memset(&s_metrics, 0, sizeof(s_metrics));
  s_lock = xSemaphoreCreateMutex();
  return s_lock ? ESP_OK : ESP_ERR_NO_MEM;
}

void aether_metrics_store_update(const aether_metrics_t *m) {
  if (!m || !s_lock) return;
  if (xSemaphoreTake(s_lock, pdMS_TO_TICKS(20)) == pdTRUE) {
    s_metrics = *m;
    xSemaphoreGive(s_lock);
  }
}

void aether_metrics_store_get(aether_metrics_t *out) {
  if (!out || !s_lock) return;
  if (xSemaphoreTake(s_lock, pdMS_TO_TICKS(20)) == pdTRUE) {
    *out = s_metrics;
    xSemaphoreGive(s_lock);
  }
}
