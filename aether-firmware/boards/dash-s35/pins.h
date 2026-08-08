#pragma once
/**
 * dash-s35 breadboard pin map (DRAFT)
 * Update after physical wiring validation in Phase 1.
 */

/* SPI display (ST7796 / ILI9488 class panels) */
#define AETHER_PIN_LCD_MOSI   11
#define AETHER_PIN_LCD_SCLK   12
#define AETHER_PIN_LCD_CS     10
#define AETHER_PIN_LCD_DC      9
#define AETHER_PIN_LCD_RST     8
#define AETHER_PIN_LCD_BL     38

/* EC11 rotary encoder */
#define AETHER_PIN_ENC_A       5
#define AETHER_PIN_ENC_B       6
#define AETHER_PIN_ENC_BTN     7

/* Status LED (optional on DevKit) */
#define AETHER_PIN_STATUS_LED  2
