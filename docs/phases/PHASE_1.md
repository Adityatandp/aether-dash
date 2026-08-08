# Phase 1 — Bench Prototype & Data POC (Weeks 1–3)

## Goals
Prove the product core on breadboard:
1. ESP32-S3 + 3.5" IPS + EC11
2. LVGL v9 AI/Dev dual-gauge dashboard
3. Web Serial streaming at ≥10 Hz from browser PWA
4. Power budget measurement path established

## Hardware (bench)
- ESP32-S3-DevKitC-1 (prefer N16R8 for PSRAM)
- 3.5" IPS 480×320 SPI (ST7796 or ILI9488 — document exact module)
- EC11 rotary encoder + push button
- USB-C cable (data capable)
- Breadboard + jumper wires
- USB power meter (for <2.5 W target)

## Firmware milestones
| Week | Milestone |
|---|---|
| 1 | Display bring-up + LVGL hello + dark theme shell |
| 2 | Protocol parser + metric store + dual gauges updating |
| 3 | Encoder actions + stability soak + power notes |

## Web app milestones
| Week | Milestone |
|---|---|
| 1 | Connect page + Web Serial permission flow |
| 2 | Encode Protocol v1 metric frames + live preview |
| 3 | Simulated metric provider + reconnect UX |

## Metric source (Phase 1 pragmatic choice)
Start with a **simulated provider** in the web app, then add one real source:
- Option A: browser-side JS sampler (limited)
- Option B: tiny local companion (Node/Python) feeding the PWA — only if needed
- Do **not** integrate full LibreHardwareMonitor matrix in week 1

## Acceptance tests (must pass)
1. Boot to LVGL dashboard without serial monitor babysitting
2. Browser connects via Web Serial and streams ≥10 Hz for 30 minutes
3. Gauges reflect changing values (sim OK)
4. Encoder rotates between brightness steps or screen focus
5. Encoder press triggers a defined action (e.g., toggle demo mode)
6. USB power measurement recorded in `docs/` or hardware validation notes
7. Board profile `dash-s35` used for build (no hardcoded pins in UI code)

## Explicitly out of scope
- MQTT / Wi‑Fi
- Custom PCB
- Gamer/ticker modes
- Multi-theme engine beyond dark
- Production enclosure

## Risks & mitigations
| Risk | Mitigation |
|---|---|
| Panel driver mismatch | Keep ST7796 + ILI9488 stubs; lock BOM early |
| LVGL frame drops at 10 Hz | Partial buffers + PSRAM; profile CPU |
| Web Serial flaky on some OS | Document Chrome/Edge only for POC |
| Pin conflicts with USB JTAG | Prefer USB-UART or explicit CDC config |
