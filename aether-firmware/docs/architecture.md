# Firmware architecture

## Goals
- Multi-board / multi-display support without UI forks
- Clean transport swap (Web Serial → WebUSB → MQTT)
- FreeRTOS-friendly metric pipeline at ≥10 Hz

## Layers
1. **HAL** — board info, display, input
2. **Protocol** — NDJSON encode/decode
3. **Metrics store** — thread-safe latest sample
4. **UI** — LVGL layouts by class S/M/L
5. **Transports** — serial/usb/mqtt tasks
6. **Services** — NVS, OTA, power helpers

## Data flow
```text
Host metrics frame
   → transport RX task
   → protocol parse
   → metrics store
   → UI task reads store @ display refresh
```

Encoder events may emit `event` frames host-side and also adjust local brightness immediately.
