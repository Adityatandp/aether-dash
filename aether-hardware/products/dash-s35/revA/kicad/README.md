# KiCad project (revA)

Place the KiCad 8 project here:

```text
aether-dash-s35.kicad_pro
schematic/
pcb/
```

## Required schematic blocks
1. ESP32-S3 module + decoupling
2. USB-C receptacle, CC resistors, ESD on D+/D-
3. 5V→3.3V LDO (thermal relief / copper pour notes)
4. Display connector (pinout locked to chosen panel)
5. EC11 A/B/BTN with RC or firmware debounce notes
6. BOOT / RESET buttons + status LED

## Design rules (starter)
- 2-layer FR4
- 0.15 mm min clearance / 0.15 mm min track (adjust to fab capability)
- Keep USB pair short and matched as practical on 2-layer
- Mounting holes aligned with enclosure standoffs

## Status
 schematics/PCB not yet drawn — Phase 2 work.
