# Hardware variant guide

Aether will offer multiple display sizes. Keep variants disciplined.

## Rules
1. One PCB per SKU — do not jumper five panels onto one board for production.
2. Reuse `common/circuits` blocks.
3. Every hardware SKU must have a matching firmware `boards/<id>/board.json`.
4. Enclosure is SKU-specific; base stand may be shared if mounting pattern matches.
5. Launch only `dash-s35`. Additional sizes after alpha demand.

## Layout classes vs SKUs
| Layout class | Typical SKUs |
|---|---|
| S | dash-mini28, community CYD |
| M | dash-s35, dash-m40 |
| L | future dash-l70 |

Firmware UI composition follows layout class; mechanical design follows SKU.
