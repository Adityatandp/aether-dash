import type { Transport, TransportStatus } from './transport'

/** Web Serial transport (Phase 1 primary path). Requires Chromium + HTTPS/localhost. */
export class WebSerialTransport implements Transport {
  readonly name = 'webserial'
  private port: SerialPort | null = null
  private reader: ReadableStreamDefaultReader<Uint8Array> | null = null
  private writer: WritableStreamDefaultWriter<Uint8Array> | null = null
  private buffer = ''
  private lineCb: ((line: string) => void) | null = null
  private statusCb: ((status: TransportStatus, detail?: string) => void) | null = null
  private reading = false

  onLine(cb: (line: string) => void): void {
    this.lineCb = cb
  }

  onStatus(cb: (status: TransportStatus, detail?: string) => void): void {
    this.statusCb = cb
  }

  private setStatus(status: TransportStatus, detail?: string) {
    this.statusCb?.(status, detail)
  }

  async connect(): Promise<void> {
    const serial = navigator.serial
    if (!serial) {
      this.setStatus('error', 'Web Serial not supported in this browser')
      throw new Error('Web Serial API unavailable')
    }

    this.setStatus('connecting')
    this.port = await serial.requestPort()
    await this.port.open({ baudRate: 115200 })
    this.writer = this.port.writable?.getWriter() ?? null
    this.reader = this.port.readable?.getReader() ?? null
    this.reading = true
    this.setStatus('connected')
    void this.readLoop()
  }

  private async readLoop() {
    if (!this.reader) return
    const decoder = new TextDecoder()
    try {
      while (this.reading) {
        const { value, done } = await this.reader.read()
        if (done) break
        if (!value) continue
        this.buffer += decoder.decode(value, { stream: true })
        let idx: number
        while ((idx = this.buffer.indexOf('\n')) >= 0) {
          const line = this.buffer.slice(0, idx)
          this.buffer = this.buffer.slice(idx + 1)
          this.lineCb?.(line)
        }
      }
    } catch (err) {
      this.setStatus('error', err instanceof Error ? err.message : String(err))
    }
  }

  async send(line: string): Promise<void> {
    if (!this.writer) throw new Error('Not connected')
    const data = new TextEncoder().encode(line.endsWith('\n') ? line : `${line}\n`)
    await this.writer.write(data)
  }

  async disconnect(): Promise<void> {
    this.reading = false
    try {
      await this.reader?.cancel()
    } catch {
      /* ignore */
    }
    try {
      this.writer?.releaseLock()
    } catch {
      /* ignore */
    }
    try {
      await this.port?.close()
    } catch {
      /* ignore */
    }
    this.reader = null
    this.writer = null
    this.port = null
    this.setStatus('disconnected')
  }
}
