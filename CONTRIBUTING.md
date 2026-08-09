# Contributing to Aether Dash

Thanks for helping build Aether Dash — an open, driverless AI/dev desk companion.

## Workspace layout

This umbrella repository currently contains three product trees:

| Path | Future standalone repo | Scope |
|---|---|---|
| `aether-firmware/` | `aether-firmware` | ESP-IDF firmware, LVGL UI, transports |
| `aether-web-app/` | `aether-web-app` | Web Serial / WebUSB PWA |
| `aether-host/` | `aether-host` | Local real PC metrics agent |
| `aether-hardware/` | `aether-hardware` | PCB, BOM, enclosure CAD |

See [docs/REPOSITORY_STRATEGY.md](docs/REPOSITORY_STRATEGY.md) for split/publish guidance.

## Development principles

1. **Board profiles first** — never hardcode one MCU/display into app logic.
2. **Protocol is the contract** — firmware schema is canonical; web types mirror it.
3. **Phase 1 = `dash-s35` only** — architecture supports more boards, but shipping focus stays narrow.
4. **Measure power** — USB-C bus-powered target is `< 2.5 W`.

## Getting started

### Firmware
```bash
cd aether-firmware
# Requires ESP-IDF v5.2+ with LVGL component support
./scripts/build_board.sh dash-s35
```

### Web app
```bash
cd aether-web-app
npm install
npm run dev
```

### Real PC metrics host
```bash
cd aether-host
npm install
npm start
```

### Hardware
Open KiCad projects under `aether-hardware/products/<sku>/rev*/kicad/`.

## Pull request checklist

- [ ] Board profile updated if pins/capabilities changed
- [ ] Protocol version bumped if wire format changed
- [ ] Docs updated (`docs/` + project README)
- [ ] No secrets / API keys committed
- [ ] Phase scope respected (don't land MQTT/gamer UI in Phase 1 PRs unless labeled `phase-3+`)

## Issue labels (suggested)

- `phase-0` `phase-1` `phase-2` `phase-3` `phase-4`
- `firmware` `web` `hardware`
- `board-profile` `protocol` `ui` `transport`
- `good-first-issue`
