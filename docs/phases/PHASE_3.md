# Phase 3 — Software Polish & Community Alpha (Weeks 7–10)

## Goals
Make the product feel real for 20 alpha users.

## Firmware
- Seamless failover: USB WebSerial primary → Wi‑Fi MQTT secondary
- NVS config for Wi‑Fi, MQTT topics, brightness, layout preset
- OTA path design (even if basic)
- Stability: overnight soak tests

## Web app (`dash.aether.dev`)
- Settings page: colors, API endpoints, device name
- Board capability detection from handshake
- Reconnect + offline last-known state
- No full visual layout editor yet (JSON/config first)

## Alpha program (India)
- 20 units to developers / creators
- Feedback form: setup friction, UI readability, encoder UX, desirability/price
- Collect unboxing + WebUSB/Serial demo clips

## Exit criteria
- Alpha kits shipped or hand-delivered
- Feedback digest published in `docs/`
- Crash/reconnect issues triaged
- Preorder landing page copy drafted

## Feature freeze for launch candidate
Allowed: bugfixes, UX polish, dark theme refinement  
Deferred: gamer mode, ticker mode, cyberpunk theme completeness
