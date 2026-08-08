import { useEffect, useMemo, useRef, useState } from 'react'
import { ConnectPage } from './pages/ConnectPage'
import { LiveDashboard } from './pages/LiveDashboard'
import { encodeHelloAck, encodeMetrics, parseLine } from './protocol/codec'
import type { HelloPayload, MetricsPayload } from './protocol/types'
import { boardFromHello, type DetectedBoard } from './devices/detectBoard'
import { metricsStore } from './metrics/store'
import { createSimulatedMetrics } from './metrics/providers/simulator'
import { WebSerialTransport } from './transports/webSerial'
import type { TransportStatus } from './transports/transport'
import './App.css'

type AppMode = 'idle' | 'device' | 'simulator'

function App() {
  const [mode, setMode] = useState<AppMode>('idle')
  const [status, setStatus] = useState('Ready')
  const [error, setError] = useState<string | undefined>()
  const [metrics, setMetrics] = useState<MetricsPayload>({})
  const [hz, setHz] = useState(0)
  const [board, setBoard] = useState<DetectedBoard | null>(null)

  const transportRef = useRef<WebSerialTransport | null>(null)
  const simTimer = useRef<number | null>(null)

  useEffect(() => {
    return metricsStore.subscribe((m, measuredHz) => {
      setMetrics(m)
      setHz(measuredHz)
    })
  }, [])

  useEffect(() => {
    return () => {
      if (simTimer.current) window.clearInterval(simTimer.current)
      void transportRef.current?.disconnect()
    }
  }, [])

  const statusLabel = useMemo(() => {
    if (mode === 'simulator') return 'Simulator streaming @ ~10 Hz'
    return status
  }, [mode, status])

  async function connectSerial() {
    setError(undefined)
    const transport = new WebSerialTransport()
    transportRef.current = transport

    transport.onStatus((s: TransportStatus, detail?: string) => {
      setStatus(detail ? `${s}: ${detail}` : s)
      if (s === 'error') setError(detail)
    })

    transport.onLine((line) => {
      const msg = parseLine(line)
      if (!msg) return
      if (msg.type === 'hello') {
        const hello = msg.payload as HelloPayload
        setBoard(boardFromHello(hello))
        void transport.send(encodeHelloAck(10))
      }
    })

    try {
      await transport.connect()
      setMode('device')
      // Until host metric collectors exist, stream simulator frames to the device.
      if (simTimer.current) window.clearInterval(simTimer.current)
      simTimer.current = window.setInterval(() => {
        const sample = createSimulatedMetrics(Date.now())
        metricsStore.update(sample)
        void transport.send(encodeMetrics(sample))
      }, 100)
    } catch (err) {
      setMode('idle')
      setError(err instanceof Error ? err.message : String(err))
    }
  }

  function startSimulator() {
    setError(undefined)
    setBoard({
      boardId: 'sim-host',
      layoutClass: 'M',
      firmware: 'web-sim',
      width: 480,
      height: 320,
    })
    setMode('simulator')
    setStatus('simulator')
    if (simTimer.current) window.clearInterval(simTimer.current)
    simTimer.current = window.setInterval(() => {
      metricsStore.update(createSimulatedMetrics(Date.now()))
    }, 100)
  }

  async function disconnect() {
    if (simTimer.current) {
      window.clearInterval(simTimer.current)
      simTimer.current = null
    }
    await transportRef.current?.disconnect()
    transportRef.current = null
    setMode('idle')
    setBoard(null)
    setStatus('Ready')
    setHz(0)
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
            status={statusLabel}
            error={error}
          />
        ) : (
          <LiveDashboard
            metrics={metrics}
            hz={hz}
            board={board}
            mode={mode === 'simulator' ? 'simulator' : 'device'}
            onDisconnect={() => void disconnect()}
          />
        )}
      </main>
    </div>
  )
}

export default App
