# Phase 1 Bring-up Guide (`dash-s35`)

## What was implemented
- ST7796 SPI display init + `esp_lvgl_port` + LVGL v9
- AI/Dev dual-gauge dashboard (CPU/GPU arcs, tokens/s, RAM/VRAM/W/temp)
- USB-Serial-JTAG Web Serial transport (NDJSON Protocol v1)
- EC11 encoder: rotate = brightness, press = toggle demo/live
- Local demo metrics when host not linked / demo mode on
- Host unit tests for protocol parser

## Wire (breadboard)
See `../aether-hardware/products/dash-s35/docs/BENCH_WIRING.md` and `boards/dash-s35/pins.h`.

## Build & flash
```bash
. $HOME/esp/esp-idf/export.sh
cd aether-firmware
./scripts/build_board.sh dash-s35
./scripts/flash_board.sh dash-s35 /dev/ttyACM0
```

First build downloads LVGL / esp_lvgl_port / esp_lcd_st7796 via Component Manager.

## Host protocol tests (no hardware)
```bash
cd aether-firmware/tests/host
make test
```

## Web app
```bash
cd aether-web-app
npm install
npm run dev
```
Chrome/Edge → **Connect via Web Serial** → select the ESP32-S3 port.

## Expected behavior
1. Device boots into dark AI/Dev UI with DEMO MODE metrics
2. Browser connect receives `hello`, sends `hello_ack` + `metrics` @ ~10 Hz
3. Push encoder to leave demo and show LIVE USB when host streams
4. Rotate encoder to change backlight

## If panel stays white/black
- Confirm controller is ST7796 (not ILI9488)
- Try lowering SPI clock in `st7796/aether_display_st7796.c` (`pclk_hz`)
- Verify BL pin PWM wiring

## Power
Record measurements in `aether-hardware/products/dash-s35/revA/validation/power-budget.md`.
