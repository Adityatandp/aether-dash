# aether-web-app

Progressive Web App for Aether Dash (`dash.aether.dev`).

Streams telemetry to devices over **Web Serial** (Phase 1) and later **WebUSB**, with no host drivers.

## Stack
- React + TypeScript + Vite
- PWA via `vite-plugin-pwa`
- Protocol mirror of `aether-proto-v1`

## Scripts
```bash
npm install
npm run dev      # local PWA (Chrome/Edge recommended)
npm run build
npm run preview
npm run lint
```

## Key folders
```text
src/
  protocol/     # types + NDJSON codec (mirrors firmware schema)
  transports/   # Web Serial (primary), WebUSB (stub)
  metrics/      # store + simulator provider
  devices/      # board handshake helpers + profiles
  pages/        # Connect + Live dashboard
```

## Phase 1 UX
1. Open app in Chrome/Edge
2. **Connect via Web Serial** (device) or **Run simulator**
3. Live AI/Dev gauges update at ~10 Hz
4. When a device sends `hello`, app replies with `hello_ack` and starts `metrics` frames

## Browser support
- Web Serial: Chromium desktop only
- Requires secure context (HTTPS or `localhost`)

## Multi-hardware
The web app does not assume one screen size. Device `hello` announces:
- `board_id`
- `layout_class` (`S|M|L`)
- display width/height
- capability flags

Future layout editor / settings pages will key off those fields.
