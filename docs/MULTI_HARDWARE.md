# Multi-hardware & multi-display plan

## Decision
Aether will **not** be locked to one ESP32 board or one screen size.

## How it works
1. **Board profiles** in firmware (`boards/<id>/board.json` + `pins.h`)
2. **HAL** isolates GPIO/panel/input details from UI
3. **Layout classes** `S|M|L` select composition, not entirely separate apps
4. **Web handshake** discovers board capabilities at connect time
5. **Hardware SKUs** are separate PCB/enclosure products sharing common circuits

## Near-term policy
| Horizon | MCU | Displays |
|---|---|---|
| Phase 1–2 | ESP32-S3 only | `dash-s35` physical |
| Phase 3 | ESP32-S3 + community boards | firmware profiles for CYD / LilyGO |
| Post-launch | ESP32-S3 (+ P4 for large RGB later) | mini28 / m40 hardware |

## Out of scope for v1
- STM32 / RP2040 ports
- Shipping five SKUs at once
- One PCB that accepts arbitrary random panels via jumpers for retail

## Adding a size later (checklist)
1. Choose panel + measure power
2. Add firmware board profile + driver
3. Map layout class
4. Design SKU PCB + enclosure
5. Add web device profile stub if needed
6. Alpha on 5–10 units before catalog launch
