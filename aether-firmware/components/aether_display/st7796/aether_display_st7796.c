#include "aether_display_priv.h"

#include "aether_board.h"
#include "pins.h"

#include "driver/spi_master.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_st7796.h"
#include "esp_log.h"

static const char *TAG = "aether_disp_st7796";

esp_err_t aether_display_st7796_init(esp_lcd_panel_io_handle_t *io_out,
                                     esp_lcd_panel_handle_t *panel_out) {
  const aether_board_info_t *board = aether_board_get();

  spi_bus_config_t buscfg = {
      .sclk_io_num = AETHER_PIN_LCD_SCLK,
      .mosi_io_num = AETHER_PIN_LCD_MOSI,
      .miso_io_num = -1,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = board->display_w * 40 * sizeof(uint16_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
      .cs_gpio_num = AETHER_PIN_LCD_CS,
      .dc_gpio_num = AETHER_PIN_LCD_DC,
      .spi_mode = 0,
      .pclk_hz = 40 * 1000 * 1000,
      .trans_queue_depth = 10,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = AETHER_PIN_LCD_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
      .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7796(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
  ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  *io_out = io_handle;
  *panel_out = panel_handle;
  ESP_LOGI(TAG, "ST7796 ready %ux%u", board->display_w, board->display_h);
  return ESP_OK;
}
