import type { MetricsPayload } from '../../protocol/types'

const DEFAULT_HOST = 'http://127.0.0.1:8787'

export function getRealHostBase(): string {
  return import.meta.env.VITE_AETHER_HOST_URL || DEFAULT_HOST
}

export async function fetchRealHostHealth(baseUrl = getRealHostBase()): Promise<boolean> {
  try {
    const res = await fetch(`${baseUrl}/health`, { cache: 'no-store' })
    if (!res.ok) return false
    const data = (await res.json()) as { ok?: boolean }
    return Boolean(data.ok)
  } catch {
    return false
  }
}

export async function fetchRealHostMetrics(
  baseUrl = getRealHostBase(),
): Promise<MetricsPayload> {
  const res = await fetch(`${baseUrl}/metrics`, { cache: 'no-store' })
  if (!res.ok) {
    throw new Error(`aether-host /metrics failed (${res.status})`)
  }
  const data = (await res.json()) as { payload?: MetricsPayload }
  if (!data.payload || typeof data.payload !== 'object') {
    throw new Error('aether-host returned invalid metrics payload')
  }
  return data.payload
}
