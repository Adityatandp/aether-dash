# aether-firmware

ESP-IDF / C++ firmware for Aether Dash devices.

## Features (roadmap-aligned)
- LVGL v9 UI with layout classes `S | M | L`
- Board profiles for multiple displays / community boards
- Web Serial / WebUSB transport handlers
- MQTT failover (Phase 3)
- FreeRTOS task architecture

## Supported boards

| Board ID | MCU | Display | Layout | Status |
|---|---|---|---|---|
| `dash-s35` | ESP32-S3 | 480×320 SPI | M | Active (Phase 1) |
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
# from aether-firmware/
./scripts/build_board.sh dash-s35

# equivalent
export AETHER_BOARD=dash-s35
idf.py set-target esp32s3
idf.py -D AETHER_BOARD=dash-s35 build
idf.py -p /dev/ttyACM0 flash monitor
```

## Architecture

```text
main/
  └── app_controller — wires transports, metrics, UI

components/
  aether_hal/        Board / display / input abstractions
  aether_display/    Panel drivers + LVGL port
  aether_input/      EC11 / buttons / touch
  aether_transport/  webserial | webusb | mqtt
  aether_protocol/   NDJSON encode/decode
  aether_metrics/    Metric store + smoothing
  aether_ui/         Screens, widgets, themes, layouts
  aether_services/   NVS config, power, OTA stubs
```

## Adding a new display / board

1. Create `boards/<id>/board.json` + `pins.h`
2. Add/select panel driver under `components/aether_display/`
3. Map `layout_class` to `S|M|L`
4. Document in `docs/boards/adding-a-board.md`
5. Build with `./scripts/build_board.sh <id>`

UI business logic must not include pin numbers or panel init.

## Protocol
Canonical docs live in repo root `docs/protocol/` during umbrella incubation, mirrored under `docs/protocol/` here.

## Phase focus
Phase 1 = Web Serial + AI/Dev dual gauge on `dash-s35` only.
