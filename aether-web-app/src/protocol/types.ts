/** Mirrored from docs/protocol/aether-proto-v1.md */

export const AETHER_PROTOCOL_MAJOR = 1 as const
export const AETHER_PROTOCOL_VERSION = '1.0'

export type LayoutClass = 'S' | 'M' | 'L'

export type MessageType =
  | 'hello'
  | 'hello_ack'
  | 'metrics'
  | 'config_set'
  | 'event'
  | 'ping'
  | 'pong'
  | 'error'

export interface Envelope<TType extends MessageType = MessageType, TPayload = unknown> {
  v: typeof AETHER_PROTOCOL_MAJOR
  type: TType
  ts: number
  payload: TPayload
}

export interface DeviceCaps {
  webserial: boolean
  webusb: boolean
  wifi_mqtt: boolean
  encoder: boolean
  touch: boolean
}

export interface HelloPayload {
  board_id: string
  fw: string
  protocol: string
  layout_class: LayoutClass
  caps: DeviceCaps
  display: { w: number; h: number; rotation?: number }
}

export interface HelloAckPayload {
  client: string
  client_fw: string
  wanted_hz: number
}

export interface MetricsPayload {
  cpu_pct?: number
  ram_pct?: number
  gpu_pct?: number
  vram_pct?: number
  tokens_per_s?: number
  pkg_watts?: number
  gpu_temp_c?: number
  label?: string
}

export interface ConfigSetPayload {
  brightness?: number
  theme?: string
  mode?: 'ai_dev' | 'gamer' | 'ticker'
}

export type HelloMessage = Envelope<'hello', HelloPayload>
export type HelloAckMessage = Envelope<'hello_ack', HelloAckPayload>
export type MetricsMessage = Envelope<'metrics', MetricsPayload>
export type ConfigSetMessage = Envelope<'config_set', ConfigSetPayload>
export type PingMessage = Envelope<'ping', Record<string, never>>
export type PongMessage = Envelope<'pong', Record<string, never>>

export type AetherMessage =
  | HelloMessage
  | HelloAckMessage
  | MetricsMessage
  | ConfigSetMessage
  | PingMessage
  | PongMessage
  | Envelope<'event', { name: string; delta?: number }>
  | Envelope<'error', { message: string }>
