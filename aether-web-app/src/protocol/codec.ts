import {
  AETHER_PROTOCOL_MAJOR,
  type AetherMessage,
  type Envelope,
  type HelloAckPayload,
  type MetricsPayload,
  type MessageType,
} from './types'

export function encodeMessage<T extends MessageType>(
  type: T,
  payload: unknown,
  ts: number = Date.now(),
): string {
  const env: Envelope = {
    v: AETHER_PROTOCOL_MAJOR,
    type,
    ts,
    payload: payload as Record<string, unknown>,
  }
  return `${JSON.stringify(env)}\n`
}

export function encodeHelloAck(wantedHz = 10, clientFw = '0.1.0-dev'): string {
  const payload: HelloAckPayload = {
    client: 'aether-web-app',
    client_fw: clientFw,
    wanted_hz: wantedHz,
  }
  return encodeMessage('hello_ack', payload)
}

export function encodeMetrics(metrics: MetricsPayload): string {
  return encodeMessage('metrics', metrics)
}

export function encodePing(): string {
  return encodeMessage('ping', {})
}

export function parseLine(line: string): AetherMessage | null {
  const trimmed = line.trim()
  if (!trimmed) return null
  try {
    const obj = JSON.parse(trimmed) as AetherMessage
    if (obj.v !== AETHER_PROTOCOL_MAJOR || typeof obj.type !== 'string') {
      return null
    }
    return obj
  } catch {
    return null
  }
}
