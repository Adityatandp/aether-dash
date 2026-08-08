import type { MetricsPayload } from '../protocol/types'
import type { DetectedBoard } from '../devices/detectBoard'

interface Props {
  metrics: MetricsPayload
  hz: number
  board?: DetectedBoard | null
  mode: 'device' | 'simulator'
  onDisconnect: () => void
}

function Gauge({ label, value, unit = '%' }: { label: string; value?: number; unit?: string }) {
  const v = value ?? 0
  return (
    <div className="gauge">
      <div className="gauge-head">
        <span>{label}</span>
        <strong>
          {Number.isFinite(v) ? v.toFixed(1) : '—'}
          {unit}
        </strong>
      </div>
      <div className="gauge-bar">
        <div className="gauge-fill" style={{ width: `${Math.max(0, Math.min(100, v))}%` }} />
      </div>
    </div>
  )
}

export function LiveDashboard({ metrics, hz, board, mode, onDisconnect }: Props) {
  return (
    <section className="panel wide">
      <div className="row">
        <div>
          <p className="eyebrow">{mode === 'simulator' ? 'Simulator' : 'Live device'}</p>
          <h1>AI / Developer Mode</h1>
          <p className="lede">
            {board
              ? `${board.boardId} · ${board.width}×${board.height} · fw ${board.firmware}`
              : 'Streaming Protocol v1 metric frames'}
          </p>
        </div>
        <div className="actions">
          <span className="pill">{hz} Hz</span>
          <button type="button" className="secondary" onClick={onDisconnect}>
            Disconnect
          </button>
        </div>
      </div>

      <div className="gauge-grid">
        <Gauge label="CPU" value={metrics.cpu_pct} />
        <Gauge label="RAM" value={metrics.ram_pct} />
        <Gauge label="GPU" value={metrics.gpu_pct} />
        <Gauge label="VRAM" value={metrics.vram_pct} />
      </div>

      <div className="stat-row">
        <div className="stat">
          <span>Tokens / sec</span>
          <strong>{metrics.tokens_per_s?.toFixed(1) ?? '—'}</strong>
        </div>
        <div className="stat">
          <span>Package W</span>
          <strong>{metrics.pkg_watts?.toFixed(1) ?? '—'}</strong>
        </div>
        <div className="stat">
          <span>GPU °C</span>
          <strong>{metrics.gpu_temp_c?.toFixed(1) ?? '—'}</strong>
        </div>
        <div className="stat">
          <span>Label</span>
          <strong>{metrics.label ?? '—'}</strong>
        </div>
      </div>
    </section>
  )
}
