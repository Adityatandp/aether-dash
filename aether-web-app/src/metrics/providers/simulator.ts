import type { MetricsPayload } from '../../protocol/types'

/** Deterministic-ish demo metrics for Phase 1 without host sensors. */
export function createSimulatedMetrics(tMs: number): MetricsPayload {
  const t = tMs / 1000
  const wave = (speed: number, offset = 0) =>
    50 + 40 * Math.sin(t * speed + offset)

  return {
    cpu_pct: clamp(wave(1.1), 0, 100),
    ram_pct: clamp(55 + 10 * Math.sin(t * 0.2), 0, 100),
    gpu_pct: clamp(wave(0.8, 1), 0, 100),
    vram_pct: clamp(60 + 15 * Math.sin(t * 0.15 + 2), 0, 100),
    tokens_per_s: clamp(18 + 12 * Math.sin(t * 0.7), 0, 80),
    pkg_watts: clamp(25 + 20 * Math.sin(t * 0.5), 5, 120),
    gpu_temp_c: clamp(55 + 12 * Math.sin(t * 0.25), 35, 95),
    label: 'sim:ollama',
  }
}

function clamp(n: number, min: number, max: number) {
  return Math.max(min, Math.min(max, Number(n.toFixed(1))))
}
