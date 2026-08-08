# dash-s35 Bench Wiring (Phase 1)

> Draft pin map — update after physical validation.

## Parts
- ESP32-S3-DevKitC-1 (N8R8 or N16R8 preferred)
- 3.5" SPI IPS (ST7796 or ILI9488 module)
- EC11 rotary encoder
- USB-C data cable
- USB power meter (recommended)

## Suggested connections

| Function | ESP32-S3 GPIO | Notes |
|---|---|---|
| LCD MOSI | 11 | SPI |
| LCD SCLK | 12 | SPI |
| LCD CS | 10 | |
| LCD DC | 9 | |
| LCD RST | 8 | |
| LCD BL | 38 | PWM brightness later |
| ENC A | 5 | |
| ENC B | 6 | |
| ENC BTN | 7 | active low w/ pull-up |

## Bring-up order
1. Power DevKit alone — confirm USB enum
2. Wire display only — flash LVGL hello
3. Add encoder — validate rotate/press events
4. Connect Web Serial from `aether-web-app`
5. Measure watts under 10 Hz stream

## Risks
- Some “3.5 inch” modules use different controllers — confirm silkscreen/IC
- Backlight current can dominate power budget
- Avoid pins reserved for flash/PSRAM/USB-JTAG on your exact module
