# Aether Dash Roadmap

```text
Phase 0 (Days 1–5)     Scaffold repos, protocol, board profile
        │
        v
Phase 1 (Weeks 1–3)    Bench POC: Web Serial + LVGL AI dashboard
        │
        v
Phase 2 (Weeks 4–6)    Custom PCB revA + enclosure for dash-s35
        │
        v
Phase 3 (Weeks 7–10)   USB↔Wi‑Fi failover, config PWA, 20 α units
        │
        v
Phase 4 (Weeks 11–16)  Preorder-gated production + D2C launch
```

## Phase summary

| Phase | Focus | Primary repos | Exit gate |
|---|---|---|---|
| 0 | Scaffold & contracts | all | Protocol v1 + `dash-s35` profile exist |
| 1 | Bench POC | firmware, web | 10 Hz / 30 min / encoder / power |
| 2 | Hardware eng | hardware (+ firmware pin map) | 10 working revA boards |
| 3 | Beta polish | firmware, web | 20 α units + feedback report |
| 4 | Launch | all + ops | Preorders validate inventory |

## Feature delivery map

| Feature | Phase |
|---|---|
| Web Serial metrics | 1 |
| LVGL AI/Dev dual gauge | 1 |
| EC11 navigation | 1 |
| Board profile HAL | 0–1 |
| Custom PCB `dash-s35` revA | 2 |
| Enclosure + stand | 2 |
| WebUSB (native) | 2–3 |
| Wi‑Fi MQTT failover | 3 |
| Config PWA (colors/endpoints) | 3 |
| Community board profiles | 3 |
| Dark theme polish | 1–3 |
| Cyberpunk / minimal themes | 3–4 |
| Gamer mode | post-β |
| System ticker mode | post-β |
| Layout visual editor | post-launch |
| `dash-mini28` / `dash-m40` hardware | post-launch (firmware-ready earlier) |

## Versioning

```text
Firmware:  fw-X.Y.Z+<board>
Web app:   web-X.Y.Z
Hardware:  hw-<sku>-revN
Protocol:  aether-proto-X.Y
```

Compatibility rule: web supports protocol `N` and `N-1`.
