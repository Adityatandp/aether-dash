# aether-hardware

Open hardware files for Aether Dash products.

## Products

| SKU | Description | Status |
|---|---|---|
| [`products/dash-s35`](products/dash-s35/) | Launch desk unit (ESP32-S3 + 3.5" 480×320 + EC11) | Active design |
| `products/dash-mini28` | Budget smaller display | Planned |
| `products/dash-m40` | Premium larger/square display | Planned |

## Layout
```text
products/<sku>/revA/
  kicad/          # schematic + PCB
  gerbers/        # manufacturing exports
  bom/            # BOM + sourcing notes
  assembly/       # CPL / assembly notes
  validation/     # bring-up + power budget
enclosures/       # OpenSCAD / STEP / print profiles
common/           # reusable circuit blocks + footprints
```

## Multi-size strategy
Hardware variants share reusable blocks under `common/circuits/`:
- USB-C + ESD
- 3V3 LDO
- EC11 encoder

Each SKU gets its own PCB + enclosure. Firmware board profiles in `aether-firmware/boards/` must stay in sync with connector/pin decisions.

## Phase plan
- **Phase 1:** breadboard only — document pin map + BOM draft
- **Phase 2:** `dash-s35` revA schematic/PCB + printable enclosure
- **Phase 4:** manufacturing package for gated production batch

## License
CERN-OHL-P intended for design files. See [`LICENSE`](LICENSE).

## Tools
- KiCad 8+
- OpenSCAD and/or Fusion 360 for enclosure
- JLCPCB / local Indian PCBA for prototype runs
