# Aether Dash

Driverless USB desk companion for live system + AI metrics.

> Plug into Chrome/Edge → stream CPU / GPU / VRAM / RAM / tokens-per-second to a beautiful LVGL dashboard. No host drivers. No elevated `.exe`.

This umbrella repository contains the full product development workspace:

| Directory | Role | Future GitHub repo |
|---|---|---|
| [`aether-firmware/`](aether-firmware/) | ESP-IDF / C++ / LVGL v9 firmware | `aether-firmware` |
| [`aether-web-app/`](aether-web-app/) | Web Serial / WebUSB PWA (`dash.aether.dev`) | `aether-web-app` |
| [`aether-host/`](aether-host/) | Local real PC metrics agent (no ESP32 needed) | `aether-host` |
| [`aether-hardware/`](aether-hardware/) | KiCad, Gerbers, BOM, enclosure CAD | `aether-hardware` |
| [`docs/`](docs/) | Product plan, roadmap, protocol, phases | stays here (or `aether-docs`) |

## Why Aether?

- **Zero-driver connectivity** via Web Serial / WebUSB
- **Multi-hardware ready** board profiles (`dash-s35`, mini, m40, community boards)
- **AI/Dev first** dashboard (tokens/s, VRAM, system load)
- **Open firmware + hardware** path for India D2C + global makers

## Launch SKU

**`dash-s35`** — ESP32-S3 · 3.5" 480×320 · EC11 encoder · USB-C bus-powered (< 2.5 W target)

Wider display sizes are supported architecturally through layout classes `S | M | L` and per-board profiles. Phase 1 builds only `dash-s35`.

## Quick start

### 1) Read the plan
- [Product Plan](docs/PRODUCT_PLAN.md)
- [Roadmap](docs/ROADMAP.md)
- [Repository Strategy](docs/REPOSITORY_STRATEGY.md)
- [Multi-hardware plan](docs/MULTI_HARDWARE.md)
- [Development setup](docs/DEVELOPMENT_SETUP.md)
- [Phase 0](docs/phases/PHASE_0.md) → [Phase 1](docs/phases/PHASE_1.md)

### 2) Test with real PC metrics (no ESP32)
```bash
# terminal 1
cd aether-host
npm install
npm start

# terminal 2
cd aether-web-app
npm install
npm run dev
```
Open `http://localhost:5173` → **Use real PC metrics**.

### 3) Web app only (dummy simulator)
```bash
cd aether-web-app
npm install
npm run dev
```
Then click **Run simulator (dummy)**.

### 4) Firmware (requires ESP-IDF v5.2+)
```bash
cd aether-firmware
./scripts/build_board.sh dash-s35
```

### 5) Hardware
See [`aether-hardware/README.md`](aether-hardware/README.md) for BOM, schematic plan, and enclosure notes.

## Protocol

Canonical wire format: **Aether Protocol v1** (NDJSON over serial)

- Spec: [`docs/protocol/aether-proto-v1.md`](docs/protocol/aether-proto-v1.md)
- Schema: [`docs/protocol/schema.json`](docs/protocol/schema.json)

## Development status

**Phase 1 — firmware/UI/transport implemented in repo.**  
On-device soak + power measurement still need physical hardware.

```text
[x] Phase 0  Scaffold & contracts
[~] Phase 1  Bench POC (code ready; hardware validation pending)
[ ] Phase 2  PCB + enclosure
[ ] Phase 3  Alpha (20 units)
[ ] Phase 4  Preorder-gated D2C launch
```

Phase 1 bring-up: [`aether-firmware/docs/PHASE1_BRINGUP.md`](aether-firmware/docs/PHASE1_BRINGUP.md)

## License

Software defaults to MIT (see [`LICENSE`](LICENSE)).  
Hardware files under `aether-hardware/` are intended for CERN-OHL (see that tree’s LICENSE).

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md).
