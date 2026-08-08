// Aether Dash S35 — parametric angled stand (draft)
// Units: mm

display_w = 85;
display_h = 58;
display_t = 6;
angle_deg = 65;
base_w = 95;
base_d = 70;
base_t = 8;
wall = 2.4;

module wedge_stand() {
  difference() {
    union() {
      // base
      translate([-base_w/2, -base_d/2, 0])
        cube([base_w, base_d, base_t]);

      // angled back support
      translate([0, 10, base_t])
        rotate([90 - angle_deg, 0, 0])
          translate([-display_w/2 - wall, 0, 0])
            cube([display_w + 2*wall, display_h + 10, wall]);
    }

    // cable channel
    translate([-8, -base_d/2 - 0.1, 2])
      cube([16, 20, 5]);
  }
}

wedge_stand();

// TODO Phase 2:
// - snap/fit for display bezel
// - encoder cutout coordinates
// - weighted base cavity
