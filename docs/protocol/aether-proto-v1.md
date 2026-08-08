# Aether Protocol v1

Canonical wire protocol between `aether-web-app` and `aether-firmware`.

**Version:** `1.0`  
**Transport framing:** newline-delimited JSON (NDJSON) over Web Serial / WebUSB CDC  
**Charset:** UTF-8  
**Endianness:** N/A (JSON text)

## Design goals
- Easy to debug in browser serial monitors
- Versioned and forward-compatible
- Board-aware handshake
- 10 Hz metric updates without huge payloads

## Message envelope

Every message is one JSON object per line:

```json
{"v":1,"type":"metrics","ts":1710000000123,"payload":{}}
```

| Field | Type | Required | Description |
|---|---|---|---|
| `v` | number | yes | Protocol major version (`1`) |
| `type` | string | yes | Message type |
| `ts` | number | yes | Unix epoch ms (sender clock) |
| `payload` | object | yes | Type-specific body |

## Message types

### `hello` (device → host)
Sent on connect / reset.

```json
{
  "v": 1,
  "type": "hello",
  "ts": 1710000000000,
  "payload": {
    "board_id": "dash-s35",
    "fw": "0.1.0",
    "protocol": "1.0",
    "layout_class": "M",
    "caps": {
      "webserial": true,
      "webusb": false,
      "wifi_mqtt": false,
      "encoder": true,
      "touch": false
    },
    "display": {"w": 480, "h": 320, "rotation": 90}
  }
}
```

### `hello_ack` (host → device)
```json
{
  "v": 1,
  "type": "hello_ack",
  "ts": 1710000000100,
  "payload": {
    "client": "aether-web-app",
    "client_fw": "0.1.0",
    "wanted_hz": 10
  }
}
```

### `metrics` (host → device)
```json
{
  "v": 1,
  "type": "metrics",
  "ts": 1710000000200,
  "payload": {
    "cpu_pct": 42.5,
    "ram_pct": 61.0,
    "gpu_pct": 33.0,
    "vram_pct": 70.0,
    "tokens_per_s": 28.4,
    "pkg_watts": 35.2,
    "gpu_temp_c": 64.0,
    "label": "ollama:llama3.2"
  }
}
```

All metric fields are optional; device renders available ones.

### `config_set` (host → device)
```json
{
  "v": 1,
  "type": "config_set",
  "ts": 1710000000300,
  "payload": {
    "brightness": 180,
    "theme": "dark",
    "mode": "ai_dev"
  }
}
```

### `event` (device → host)
Encoder / button events.

```json
{
  "v": 1,
  "type": "event",
  "ts": 1710000000400,
  "payload": {
    "name": "encoder_rotate",
    "delta": -1
  }
}
```

### `ping` / `pong`
Keepalive for soak tests.

## Rate guidance
- Metrics target: **10 Hz**
- Max line length: **1024 bytes** (reject/ignore larger)
- Device may downsample UI redraw independently of intake rate

## Compatibility
- Minor additions to `payload` are non-breaking
- Removing/renaming fields or changing `type` semantics requires protocol bump
- Web app should accept unknown fields

## Schema file
See [`schema.json`](./schema.json) (mirrored in firmware docs).
