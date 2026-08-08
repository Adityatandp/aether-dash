import { isWebSerialSupported } from '../transports/transport'

interface Props {
  onConnectSerial: () => void
  onStartSimulator: () => void
  status: string
  error?: string
}

export function ConnectPage({ onConnectSerial, onStartSimulator, status, error }: Props) {
  const serialOk = isWebSerialSupported()

  return (
    <section className="panel">
      <p className="eyebrow">dash.aether.dev</p>
      <h1>Connect your Aether Dash</h1>
      <p className="lede">
        Stream live AI/system metrics over Web Serial — no host drivers, no elevated installers.
      </p>

      <div className="actions">
        <button type="button" disabled={!serialOk} onClick={onConnectSerial}>
          Connect via Web Serial
        </button>
        <button type="button" className="secondary" onClick={onStartSimulator}>
          Run simulator (no device)
        </button>
      </div>

      {!serialOk && (
        <p className="warn">
          Web Serial requires Chrome or Edge on desktop (HTTPS or localhost).
        </p>
      )}

      <dl className="meta">
        <div>
          <dt>Status</dt>
          <dd>{status}</dd>
        </div>
        <div>
          <dt>Protocol</dt>
          <dd>aether-proto-1.0</dd>
        </div>
        <div>
          <dt>Phase 1 board</dt>
          <dd>dash-s35</dd>
        </div>
      </dl>

      {error ? <p className="error">{error}</p> : null}
    </section>
  )
}
