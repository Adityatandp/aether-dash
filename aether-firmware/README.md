# aether-firmware

ESP-IDF / C++ firmware for Aether Dash devices.

## Phase 1 status
Implemented for **`dash-s35`** breadboard POC:
- LVGL v9 AI/Dev dual-gauge UI
- ST7796 SPI via `esp_lcd` + `esp_lvgl_port`
- Web Serial over USB-Serial-JTAG (Protocol v1)
- EC11 encoder (brightness + demo toggle)
- Demo metrics generator for offline bring-up

See [`docs/PHASE1_BRINGUP.md`](docs/PHASE1_BRINGUP.md).

## Features (roadmap-aligned)
- LVGL v9 UI with layout classes `S | M | L`
- Board profiles for multiple displays / community boards
- Web Serial / WebUSB transport handlers
- MQTT failover (Phase 3)
- FreeRTOS task architecture

## Supported boards

| Board ID | MCU | Display | Layout | Status |
|---|---|---|---|---|
| `dash-s35` | ESP32-S3 | 480×320 SPI | M | **Phase 1 active** |
| `dash-mini28` | ESP32-S3 | 320×240 | S | Profile stub |
| `dash-m40` | ESP32-S3 | 480×480+ | M/L | Profile stub |
| `cyd-2432` | ESP32 | 320×240 CYD | S | Community stub |
| `lilygo-tdisplay-s3` | ESP32-S3 | LilyGO | S/M | Community stub |

## Requirements
- ESP-IDF **v5.2+**
- Python 3.10+
- Target: `esp32s3` for launch SKU

## Build

```bash
cd aether-firmware
./scripts/build_board.sh dash-s35
idf.py -D AETHER_BOARD=dash-s35 build
idf.py -p /dev/ttyACM0 flash monitor
```

## Host tests (no ESP-IDF required)

```bash
cd tests/host
make test
```

## Architecture

```text
main/
  └── app_controller — wires transports, metrics, UI, encoder

components/
  aether_hal/        Board / display / input abstractions
  aether_display/    ST7796 + LVGL port
  aether_input/      EC11 encoder
  aether_transport/  webserial (Phase 1)
  aether_protocol/   NDJSON encode/decode
  aether_metrics/    Metric store
  aether_ui/         AI/Dev screen + dark theme
  aether_services/   NVS/OTA stubs (later)
```

## Adding a new display / board

1. Create `boards/<id>/board.json` + `pins.h`
2. Add/select panel driver under `components/aether_display/`
3. Map `layout_class` to `S|M|L`
4. Document in `docs/boards/adding-a-board.md`
5. Build with `./scripts/build_board.sh <id>`
