# Power budget — dash-s35

Target: **< 2.5 W** USB-C bus-powered.

| Rail / block | Estimate (mA @5V) | Notes |
|---|---|---|
| ESP32-S3 active + radio off | 80–150 | USB serial path |
| Display panel + backlight mid | 150–350 | Dominates budget |
| Passives / LDO loss | 20–40 | |
| **Total mid** | **≈ 250–540 mA** | ≈ 1.25–2.7 W |

## Measurements (fill during Phase 1/2)
| Condition | Watts | Date | Meter |
|---|---|---|---|
| Idle UI, BL 50% |  |  |  |
| 10 Hz metrics stream |  |  |  |
| Wi‑Fi on (Phase 3) |  |  |  |
