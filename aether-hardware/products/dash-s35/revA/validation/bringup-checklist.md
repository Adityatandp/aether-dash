# dash-s35 revA Bring-up Checklist

- [ ] Visual inspection: orientation, bridges, connector seating
- [ ] Resistance check 3V3↔GND (not shorted)
- [ ] USB enumerate on host
- [ ] Flash firmware `AETHER_BOARD=dash-s35`
- [ ] Console shows board hello / app start logs
- [ ] Display shows LVGL dashboard
- [ ] Encoder rotate changes brightness or focus
- [ ] Encoder press triggers action
- [ ] Web Serial connect from `aether-web-app`
- [ ] Metrics update ≥10 Hz for 10 minutes
- [ ] Soak 30 minutes without hang
- [ ] USB power meter reading recorded in `power-budget.md`
