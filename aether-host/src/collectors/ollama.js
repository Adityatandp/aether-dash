const OLLAMA_HOST = process.env.OLLAMA_HOST || 'http://127.0.0.1:11434'
const LMSTUDIO_HOST = process.env.LMSTUDIO_HOST || 'http://127.0.0.1:1234'

async function fetchJson(url, timeoutMs = 800) {
  const ctrl = new AbortController()
  const t = setTimeout(() => ctrl.abort(), timeoutMs)
  try {
    const res = await fetch(url, { signal: ctrl.signal })
    if (!res.ok) return null
    return await res.json()
  } catch {
    return null
  } finally {
    clearTimeout(t)
  }
}

/**
 * Detect running local AI model label.
 * tokens_per_s is best-effort and often unavailable without intercepting generation streams.
 */
export async function collectAiMetrics() {
  // Ollama running models
  const ps = await fetchJson(`${OLLAMA_HOST}/api/ps`)
  if (ps && Array.isArray(ps.models) && ps.models.length > 0) {
    const m = ps.models[0]
    const name = m.name || m.model || 'ollama'
    const out = { label: `ollama:${name}` }

    // Some builds expose throughput-ish fields; keep defensive.
    const tps = Number(m.tokens_per_second || m.token_per_second || m.throughput)
    if (Number.isFinite(tps) && tps >= 0) {
      out.tokens_per_s = Number(tps.toFixed(1))
    }
    return out
  }

  // LM Studio OpenAI-compatible models list (label only)
  const lms = await fetchJson(`${LMSTUDIO_HOST}/v1/models`)
  if (lms && Array.isArray(lms.data) && lms.data.length > 0) {
    const id = lms.data[0].id || 'model'
    return { label: `lmstudio:${id}` }
  }

  return {}
}
