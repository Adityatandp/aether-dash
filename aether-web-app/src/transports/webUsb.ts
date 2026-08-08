import type { Transport, TransportStatus } from './transport'

/**
 * WebUSB transport placeholder (Phase 2–3).
 * Prefer Web Serial for Phase 1 bring-up.
 */
export class WebUsbTransport implements Transport {
  readonly name = 'webusb'
  private statusCb: ((status: TransportStatus, detail?: string) => void) | null = null

  onLine(_cb: (line: string) => void): void {}

  onStatus(cb: (status: TransportStatus, detail?: string) => void): void {
    this.statusCb = cb
  }

  async connect(): Promise<void> {
    this.statusCb?.('error', 'WebUSB transport not implemented yet (Phase 2–3)')
    throw new Error('WebUSB transport not implemented yet')
  }

  async disconnect(): Promise<void> {
    this.statusCb?.('disconnected')
  }

  async send(_line: string): Promise<void> {
    throw new Error('WebUSB transport not implemented yet')
  }
}
