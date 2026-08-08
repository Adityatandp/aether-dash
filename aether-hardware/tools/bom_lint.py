#!/usr/bin/env python3
"""Tiny BOM sanity checker for Aether hardware CSVs."""

from __future__ import annotations

import csv
import sys
from pathlib import Path

REQUIRED = {"Item", "Qty"}


def lint(path: Path) -> int:
    errors = 0
    with path.open(newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        if not reader.fieldnames:
            print(f"{path}: missing header")
            return 1
        missing = REQUIRED - set(reader.fieldnames)
        if missing:
            print(f"{path}: missing columns {sorted(missing)}")
            errors += 1
        for i, row in enumerate(reader, start=2):
            if not (row.get("Item") or "").strip():
                print(f"{path}:{i}: empty Item")
                errors += 1
                continue
            qty = (row.get("Qty") or "").strip()
            if not qty.isdigit() or int(qty) <= 0:
                print(f"{path}:{i}: invalid Qty={qty!r}")
                errors += 1
    return errors


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print("Usage: bom_lint.py <bom.csv> [more.csv...]")
        return 2
    total = 0
    for arg in argv[1:]:
        total += lint(Path(arg))
    if total:
        print(f"FAILED with {total} issue(s)")
        return 1
    print("BOM OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
