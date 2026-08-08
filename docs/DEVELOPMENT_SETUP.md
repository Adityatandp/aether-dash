# Development Setup

## Prerequisites
- Git
- Node.js 22+
- Python 3.10+
- ESP-IDF v5.2+ (firmware)
- Chrome or Edge (Web Serial)
- KiCad 8+ (hardware, Phase 2)
- Optional: OpenSCAD

## Clone
```bash
git clone https://github.com/Adityatandp/aether-dash.git
cd aether-dash
```

## Web app
```bash
cd aether-web-app
npm install
npm run dev
```
Open `http://localhost:5173` in Chrome/Edge.

## Firmware
```bash
# Install ESP-IDF then:
. $HOME/esp/esp-idf/export.sh
cd aether-firmware
./scripts/build_board.sh dash-s35
./scripts/flash_board.sh dash-s35 /dev/ttyACM0
```

## Hardware
- Read `aether-hardware/products/dash-s35/docs/BENCH_WIRING.md`
- Order BOM items from `revA/bom/bom.csv`
- OpenSCAD draft: `enclosures/dash-s35/openscad/dash_s35_stand.scad`

## Suggested Phase 1 daily loop
1. Firmware display/UI change
2. Flash device
3. Connect from web app (or simulator)
4. Note Hz stability + power

## Repo split later
See [REPOSITORY_STRATEGY.md](REPOSITORY_STRATEGY.md).
