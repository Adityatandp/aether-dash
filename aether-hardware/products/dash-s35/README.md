# dash-s35 — Launch Hardware

**Target:** ESP32-S3 module · 3.5" IPS 480×320 · EC11 · USB-C bus-powered

## Current revision
- `revA/` — first custom PCB + enclosure path (Phase 2)

## Phase 1 (breadboard)
Use DevKit wiring documented in:
- firmware `boards/dash-s35/pins.h`
- [`docs/BENCH_WIRING.md`](docs/BENCH_WIRING.md)
- [`revA/bom/bom.csv`](revA/bom/bom.csv) (prototype shopping list)

## Design goals
- Instant-on LVGL UI path supported by clean power rail
- USB-C data + power with ESD protection
- Encoder on front/side of angled enclosure
- Keep peak consumption under **2.5 W**

## Multi-display note
This SKU is layout class **M**. Other sizes are separate products, not stuffed onto this PCB.
