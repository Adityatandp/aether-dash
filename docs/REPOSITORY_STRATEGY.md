# Repository Strategy

## Current state (umbrella monorepo)

GitHub: `Adityatandp/aether-dash`

```text
aether-dash/
├── aether-firmware/     # future: aether-firmware
├── aether-web-app/      # future: aether-web-app
├── aether-hardware/     # future: aether-hardware
└── docs/                # product + phase plans (canonical during incubation)
```

This keeps early development fast (one PR can touch protocol + firmware + web types).

## Target multi-repo layout

Create these GitHub repositories when Phase 1 exits:

| Repo | Visibility | License intent |
|---|---|---|
| `aether-firmware` | Public | Apache-2.0 or MIT |
| `aether-web-app` | Public | MIT |
| `aether-hardware` | Public | CERN-OHL-P (preferred) |
| `aether-docs` (optional) | Public | CC-BY-4.0 |

## Why start umbrella, then split?

1. Protocol changes are atomic during POC.
2. No submodule pain before first working demo.
3. Easy contributor onboarding for founders.
4. Clear extraction boundary already encoded by folder names.

## Extraction checklist (later)

```bash
# Example: extract firmware history into new repo
git subtree split -P aether-firmware -b split-firmware
# In new empty GitHub repo:
git push <new-remote> split-firmware:main
```

Repeat for `aether-web-app` and `aether-hardware`.

After split:
1. Move canonical protocol docs into `aether-firmware/docs/protocol/`
2. Generate/copy TypeScript types into `aether-web-app/src/protocol/`
3. Add CI badges to umbrella README pointing at each repo
4. Pin release compatibility matrix in `docs/ROADMAP.md`

## Cross-repo contract

- **Canonical schema:** `aether-firmware/docs/protocol/schema.json` (also mirrored at `docs/protocol/`)
- **Handshake:** device announces `board_id`, `protocol`, `layout_class`, `capabilities`
- **Release trains:** tag firmware/web together when protocol minor bumps
