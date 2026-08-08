export type TransportStatus = 'disconnected' | 'connecting' | 'connected' | 'error'

export interface Transport {
  readonly name: string
  connect(): Promise<void>
  disconnect(): Promise<void>
  send(line: string): Promise<void>
  onLine(cb: (line: string) => void): void
  onStatus(cb: (status: TransportStatus, detail?: string) => void): void
}

export function isWebSerialSupported(): boolean {
  return typeof navigator !== 'undefined' && 'serial' in navigator
}

export function isWebUsbSupported(): boolean {
  return typeof navigator !== 'undefined' && 'usb' in navigator
}
