#!/usr/bin/env bash
set -euo pipefail

BOARD="${1:-dash-s35}"
PORT="${2:-/dev/ttyACM0}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

if [[ -z "${IDF_PATH:-}" ]]; then
  echo "ESP-IDF not loaded. Run: . \$HOME/esp/esp-idf/export.sh"
  exit 1
fi

cd "${ROOT}"
idf.py -D "AETHER_BOARD=${BOARD}" -p "${PORT}" flash monitor
