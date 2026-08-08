# Aether Dash — Product Development Plan

**Codename:** Aether Dash  
**Promise:** Driverless USB desk display for live CPU/GPU/VRAM/RAM and AI token metrics, with open firmware and multi-size hardware roadmap.

## Vision

A plug-and-play ESP-family desk companion that streams metrics from a browser (Web Serial / WebUSB) without host drivers or elevated `.exe` processes, with optional Wi‑Fi MQTT later.

## Beachhead

- **Primary user:** AI/LLM tinkerers, indie hackers, developers in India
- **Launch SKU:** `dash-s35` — ESP32-S3 + 3.5" 480×320 + EC11
- **Primary mode:** Developer / AI Mode
- **Price band (India target):** ₹4,499–₹5,999

## Non-goals (Phase 1)

- Full theme marketplace
- Gamer / ticker modes
- Multi-MCU families beyond ESP32-S3
- 500-unit production before preorder validation
- Full visual layout editor

## Multi-hardware strategy

Aether supports a **wide display range** through:

1. **Board profiles** (`boards/<sku>/board.json`)
2. **HAL layers** (board / display / input)
3. **Layout classes** `S | M | L` (responsive composition, shared widgets/themes)

| SKU | Display class | Target | Status |
|---|---|---|---|
| `dash-s35` | M (480×320) | Launch | Active |
| `dash-mini28` | S (320×240) | Budget | Planned |
| `dash-m40` | M/L (480×480+) | Premium | Planned |
| `dash-l70` | L (800×480+) | Shelf/wall | Future (maybe ESP32-P4) |
| Community boards | varies | DIY | Scaffolded |

## System architecture

```text
Host browser (aether-web-app)
    │ Web Serial / WebUSB  (Protocol v1)
    ▼
ESP device (aether-firmware)
    ├── Transport layer
    ├── Metric store
    ├── LVGL UI (layout class from board profile)
    └── Inputs (EC11 / touch / buttons)
```

## Success metrics

### Phase 1 exit
- ≥10 Hz stable metric stream for 30 minutes
- LVGL AI dual-gauge usable on `dash-s35`
- Encoder changes brightness/screen
- USB power measured < 2.5 W target

### Commercial gate before Phase 4 volume
- Working α units filmed by creators
- ~50 paid deposits / strong waitlist signal before 500-unit SMT

## Related docs

- [ROADMAP.md](ROADMAP.md)
- [REPOSITORY_STRATEGY.md](REPOSITORY_STRATEGY.md)
- [phases/PHASE_0.md](phases/PHASE_0.md) … [PHASE_4.md](phases/PHASE_4.md)
- [protocol/aether-proto-v1.md](protocol/aether-proto-v1.md)
- [business/UNIT_ECONOMICS.md](business/UNIT_ECONOMICS.md)
