# Design rules (starter)

## Electrical
- USB-C bus-powered only for v1 SKUs
- Protect D+/D- with ESD array near connector
- Decouple ESP32-S3 module per Espressif reference
- Provide test points: 5V (VBUS), 3V3, GND, TX/RX

## Mechanical
- Angled display for desk viewing
- Encoder reachable without shifting device
- Cable exit that doesn’t tip the stand
- Standoff pattern documented before enclosure freeze

## Manufacturing
- Prefer JLCPCB basic parts for passives
- Lock display connector before any fab order
- Assemble 10 boards before changing to revB
