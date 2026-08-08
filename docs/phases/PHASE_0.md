# Phase 0 — Scaffold & Contracts (Days 1–5)

## Goals
- Establish umbrella repository structure
- Freeze Protocol v1 draft
- Define launch board profile `dash-s35`
- Document multi-board / multi-display architecture
- Agree Phase 1 acceptance criteria

## Deliverables
- [x] `aether-firmware/`, `aether-web-app/`, `aether-hardware/` trees
- [x] Product plan + roadmap docs
- [x] Protocol v1 markdown + JSON schema
- [x] `dash-s35` board.json + pin map draft
- [ ] Local ESP-IDF toolchain install on developer machines
- [ ] First breadboard wiring photo/doc

## Tasks

### Day 1–2
1. Confirm BOM order for DevKit + 3.5" IPS + EC11
2. Install ESP-IDF v5.2+, Node 20+/22
3. Read `docs/PRODUCT_PLAN.md` and protocol docs

### Day 3–4
1. Validate board pin assumptions on breadboard
2. Update `boards/dash-s35/pins.h` with measured wiring
3. Run web app scaffold (`npm install && npm run dev`)

### Day 5
1. Phase 0 review checklist complete
2. Open Phase 1 implementation issues

## Exit criteria
- All three project READMEs usable by a new contributor
- Protocol v1 reviewed (even if stub parsers)
- `dash-s35` profile exists with layout class `M`
- No blocking unknowns for bench wiring

## Owners
- Firmware lead: board profile + HAL stubs
- Web lead: transport interface + protocol types
- Hardware lead: BOM + pin map documentation
