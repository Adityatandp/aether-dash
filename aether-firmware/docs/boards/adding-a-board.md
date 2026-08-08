# Adding a Board Profile

Aether firmware is multi-hardware by design. Follow this checklist.

## 1. Create profile directory
```text
boards/<board-id>/
  board.json
  pins.h
  sdkconfig.defaults   # optional overrides
```

## 2. Fill `board.json`
Required fields:
- `id`, `mcu`, `display.{driver,width,height,bus,layout_class}`
- `inputs[]`, `transports[]`

`layout_class` must be one of `S|M|L`.

## 3. Implement / select drivers
- Display driver in `components/aether_display/<panel>/`
- Input support in `components/aether_input/`
- Do not fork UI screens for a new board unless layout class needs new composition

## 4. Build
```bash
./scripts/build_board.sh <board-id>
```

## 5. Document
- Add row to firmware README board table
- Note any quirks in `docs/boards/<board-id>.md`

## Rules
- No GPIO numbers inside `aether_ui`
- No panel init inside metric/protocol code
- Prefer extending HAL over copy-paste main.cpp
