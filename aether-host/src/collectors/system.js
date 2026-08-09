import si from 'systeminformation'

function clampPct(n) {
  if (!Number.isFinite(n)) return undefined
  return Math.max(0, Math.min(100, Number(n.toFixed(1))))
}

export async function collectSystemMetrics() {
  const [load, mem] = await Promise.all([si.currentLoad(), si.mem()])

  const cpu = clampPct(load.currentLoad)
  const ram = mem.total > 0 ? clampPct((mem.active / mem.total) * 100) : undefined

  return {
    cpu_pct: cpu,
    ram_pct: ram,
  }
}
