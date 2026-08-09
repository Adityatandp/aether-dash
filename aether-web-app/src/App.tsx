import { useEffect, useMemo, useRef, useState } from 'react'
import { ConnectPage } from './pages/ConnectPage'
import { LiveDashboard } from './pages/LiveDashboard'
import { encodeHelloAck, encodeMetrics, parseLine } from './protocol/codec'
import type { HelloPayload, MetricsPayload } from './protocol/types'
import { boardFromHello, type DetectedBoard } from './devices/detectBoard'
import { metricsStore } from './metrics/store'
import { createSimulatedMetrics } from './metrics/providers/simulator'
import {
  fetchRealHostHealth,
  fetchRealHostMetrics,
  getRealHostBase,
} from './metrics/providers/realHost'
import { WebSerialTransport } from './transports/webSerial'
import type { TransportStatus } from './transports/transport'
import './App.css'

type AppMode = 'idle' | 'device' | 'simulator' | 'real'

function App() {
  const [mode, setMode] = useState<AppMode>('idle')
  const [status, setStatus] = useState('Ready')
  const [error, setError] = useState<string | undefined>()
  const [metrics, setMetrics] = useState<MetricsPayload>({})
  const [hz, setHz] = useState(0)
  const [board, setBoard] = useState<DetectedBoard | null>(null)
  const [lastEvent, setLastEvent] = useState<string>('')

  const transportRef = useRef<WebSerialTransport | null>(null)
  const streamTimer = useRef<number | null>(null)
  const modeRef = useRef<AppMode>('idle')

  useEffect(() => {
    modeRef.current = mode
  }, [mode])

  useEffect(() => {
    return metricsStore.subscribe((m, measuredHz) => {
      setMetrics(m)
      setHz(measuredHz)
    })
  }, [])

  useEffect(() => {
    return () => {
      if (streamTimer.current) window.clearInterval(streamTimer.current)
      void transportRef.current?.disconnect()
    }
  }, [])

  const statusLabel = useMemo(() => {
    if (mode === 'simulator') return 'Simulator streaming @ ~10 Hz'
    if (mode === 'real') return `Real metrics from ${getRealHostBase()} @ ~10 Hz`
    if (lastEvent) return `${status} · ${lastEvent}`
    return status
  }, [mode, status, lastEvent])

  function stopStreamTimer() {
    if (streamTimer.current) {
      window.clearInterval(streamTimer.current)
      streamTimer.current = null
    }
  }

  function startDeviceStream(transport: WebSerialTransport) {
    stopStreamTimer()
    streamTimer.current = window.setInterval(() => {
      if (modeRef.current !== 'device') return
      // Prefer forwarding latest real/sim sample if present; otherwise generate sim.
      const latest = metricsStore.get()
      const sample =
        latest.cpu_pct != null || latest.ram_pct != null
          ? latest
          : createSimulatedMetrics(Date.now())
      metricsStore.update(sample)
      void transport.send(encodeMetrics(sample)).catch((err: unknown) => {
        setError(err instanceof Error ? err.message : String(err))
      })
    }, 100)
  }

  async function connectSerial() {
    setError(undefined)
    setLastEvent('')
    const transport = new WebSerialTransport()
    transportRef.current = transport

    transport.onStatus((s: TransportStatus, detail?: string) => {
      setStatus(detail ? `${s}: ${detail}` : s)
      if (s === 'error') {
        setError(detail)
        stopStreamTimer()
        setMode('idle')
      }
      if (s === 'disconnected' && modeRef.current === 'device') {
        stopStreamTimer()
        setMode('idle')
        setStatus('Disconnected — reconnect when ready')
      }
    })

    transport.onLine((line) => {
      const msg = parseLine(line)
      if (!msg) return
      if (msg.type === 'hello') {
        const hello = msg.payload as HelloPayload
        setBoard(boardFromHello(hello))
        setLastEvent(`hello from ${hello.board_id}`)
        void transport.send(encodeHelloAck(10))
      } else if (msg.type === 'event') {
        const payload = msg.payload as { name?: string; delta?: number }
        setLastEvent(`${payload.name ?? 'event'}${payload.delta != null ? ` ${payload.delta}` : ''}`)
      } else if (msg.type === 'pong') {
        setLastEvent('pong')
      }
    })

    try {
      await transport.connect()
      setMode('device')
      startDeviceStream(transport)
    } catch (err) {
      setMode('idle')
      setError(err instanceof Error ? err.message : String(err))
    }
  }

  function startSimulator() {
    setError(undefined)
    setLastEvent('simulator')
    setBoard({
      boardId: 'sim-host',
      layoutClass: 'M',
      firmware: 'web-sim',
      width: 480,
      height: 320,
    })
    setMode('simulator')
    setStatus('simulator')
    stopStreamTimer()
    streamTimer.current = window.setInterval(() => {
      metricsStore.update(createSimulatedMetrics(Date.now()))
    }, 100)
  }

  async function startRealMetrics() {
    setError(undefined)
    setLastEvent('')
    const base = getRealHostBase()
    const healthy = await fetchRealHostHealth(base)
    if (!healthy) {
      setError(
        `Cannot reach aether-host at ${base}. Run: cd aether-host && npm install && npm start`,
      )
      return
    }

    setBoard({
      boardId: 'pc-host',
      layoutClass: 'M',
      firmware: 'aether-host',
      width: 480,
      height: 320,
    })
    setMode('real')
    setStatus('real-host')
    stopStreamTimer()

    streamTimer.current = window.setInterval(() => {
      if (modeRef.current !== 'real') return
      void fetchRealHostMetrics(base)
        .then((sample) => {
          metricsStore.update(sample)
        })
        .catch((err: unknown) => {
          setError(err instanceof Error ? err.message : String(err))
          stopStreamTimer()
          setMode('idle')
          setStatus('Real metrics disconnected')
        })
    }, 100)
  }

  async function disconnect() {
    stopStreamTimer()
    await transportRef.current?.disconnect()
    transportRef.current = null
    setMode('idle')
    setBoard(null)
    setStatus('Ready')
    setHz(0)
    setLastEvent('')
  }

  return (
    <div className="app-shell">
      <header className="topbar">
        <div className="brand">Aether Dash</div>
        <nav>
          <a href="https://github.com/Adityatandp/aether-dash" target="_blank" rel="noreferrer">
            GitHub
          </a>
        </nav>
      </header>

      <main>
        {mode === 'idle' ? (
          <ConnectPage
            onConnectSerial={() => void connectSerial()}
            onStartSimulator={startSimulator}
            onStartRealMetrics={() => void startRealMetrics()}
            status={statusLabel}
            error={error}
          />
        ) : (
          <LiveDashboard
            metrics={metrics}
            hz={hz}
            board={board}
            mode={mode === 'simulator' ? 'simulator' : mode === 'real' ? 'real' : 'device'}
            onDisconnect={() => void disconnect()}
          />
        )}
      </main>
    </div>
  )
}

export default App
