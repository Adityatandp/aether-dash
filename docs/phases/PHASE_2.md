# Phase 2 — Hardware Engineering (Weeks 4–6)

## Goals
Turn the breadboard POC into a manufacturable `dash-s35` revA design.

## Deliverables
- 2-layer KiCad schematic + PCB
- BOM with India/JLCPCB sourcing notes
- Enclosure OpenSCAD/STEP for angled desk mount + weighted base concept
- 10-unit PCBA prototype order plan
- Firmware pin map updated for production connector

## Schematic blocks
1. ESP32-S3 module
2. USB-C (CC resistors, ESD, power path)
3. 3.3V LDO / power integrity
4. Display connector (FFC/FPC or pin header — decide before layout)
5. EC11 circuit + debounce strategy
6. Boot/reset buttons, status LED

## PCB constraints
- 2-layer for cost (revA)
- Keep USB D+/D- short and impedance-aware as practical on 2-layer
- Clearance for enclosure standoffs
- Test points for 3V3, GND, UART TX/RX

## Enclosure goals
- Angled display (~60–70°)
- Encoder accessible on side/front
- Cable exit for USB-C
- Printable on consumer FDM first; evaluate resin later

## Exit criteria
- Gerbers + BOM linted
- At least DRC clean KiCad project committed
- Assembly notes for first 10 boards
- Bring-up checklist filled for first powered board

## Do not expand yet
- Second SKU PCB (`mini28`/`m40`) — firmware profiles only
- Aluminum CNC enclosure
