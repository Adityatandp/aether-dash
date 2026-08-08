#!/usr/bin/env bash
set -euo pipefail

BOARD="${1:-dash-s35}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BOARD_DIR="${ROOT}/boards/${BOARD}"

if [[ ! -f "${BOARD_DIR}/board.json" ]]; then
  echo "Unknown board: ${BOARD}"
  echo "Available:"
  find "${ROOT}/boards" -name board.json | sed "s#${ROOT}/boards/##;s#/board.json##"
  exit 1
fi

if [[ -z "${IDF_PATH:-}" ]]; then
  echo "ESP-IDF not loaded. Run: . \$HOME/esp/esp-idf/export.sh"
  echo "Board profile '${BOARD}' is valid; build skipped in this environment."
  exit 0
fi

cd "${ROOT}"
idf.py set-target esp32s3
idf.py -D "AETHER_BOARD=${BOARD}" build
echo "Built firmware for board=${BOARD}"
