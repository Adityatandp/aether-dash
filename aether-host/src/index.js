import cors from 'cors'
import express from 'express'
import { collectAiMetrics } from './collectors/ollama.js'
import { collectNvidiaMetrics } from './collectors/nvidia.js'
import { collectSystemMetrics } from './collectors/system.js'

const PORT = Number(process.env.PORT || 8787)
const app = express()

app.use(
  cors({
    origin: [/localhost:\d+$/, /127\.0\.0\.1:\d+$/],
  }),
)

async function sampleMetrics() {
  const [sys, nvidia, ai] = await Promise.all([
    collectSystemMetrics(),
    collectNvidiaMetrics(),
    collectAiMetrics(),
  ])

  const payload = {
    ...sys,
    ...nvidia,
    ...ai,
  }

  return {
    v: 1,
    type: 'metrics',
    ts: Date.now(),
    payload,
    source: {
      agent: 'aether-host',
      nvidia: Object.keys(nvidia).length > 0,
      ai: Boolean(payload.label),
    },
  }
}

app.get('/health', (_req, res) => {
  res.json({
    ok: true,
    service: 'aether-host',
    version: '0.1.0-dev',
    port: PORT,
  })
})

app.get('/metrics', async (_req, res) => {
  try {
    const sample = await sampleMetrics()
    res.json(sample)
  } catch (err) {
    res.status(500).json({
      ok: false,
      error: err instanceof Error ? err.message : String(err),
    })
  }
})

app.listen(PORT, '127.0.0.1', () => {
  console.log(`aether-host listening on http://127.0.0.1:${PORT}`)
  console.log(`  health : http://127.0.0.1:${PORT}/health`)
  console.log(`  metrics: http://127.0.0.1:${PORT}/metrics`)
})
