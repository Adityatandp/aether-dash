import { execFile } from 'node:child_process'
import { promisify } from 'node:util'

const execFileAsync = promisify(execFile)

function parseNumber(value) {
  const n = Number(String(value).trim())
  return Number.isFinite(n) ? n : undefined
}

/**
 * Best-effort NVIDIA metrics via nvidia-smi.
 * Returns {} if nvidia-smi is missing or fails.
 */
export async function collectNvidiaMetrics() {
  try {
    const { stdout } = await execFileAsync(
      'nvidia-smi',
      [
        '--query-gpu=utilization.gpu,memory.used,memory.total,temperature.gpu,power.draw',
        '--format=csv,noheader,nounits',
      ],
      { timeout: 1500, windowsHide: true },
    )

    const line = stdout
      .split(/\r?\n/)
      .map((l) => l.trim())
      .find(Boolean)
    if (!line) return {}

    const parts = line.split(',').map((p) => p.trim())
    const util = parseNumber(parts[0])
    const memUsed = parseNumber(parts[1])
    const memTotal = parseNumber(parts[2])
    const temp = parseNumber(parts[3])
    const watts = parseNumber(parts[4])

    const out = {}
    if (util != null) out.gpu_pct = Number(util.toFixed(1))
    if (memUsed != null && memTotal != null && memTotal > 0) {
      out.vram_pct = Number(((memUsed / memTotal) * 100).toFixed(1))
    }
    if (temp != null) out.gpu_temp_c = Number(temp.toFixed(1))
    if (watts != null) out.pkg_watts = Number(watts.toFixed(1))
    return out
  } catch {
    return {}
  }
}
