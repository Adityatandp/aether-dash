import type { DeviceCaps } from '../protocol/types'

export function describeCaps(caps: DeviceCaps): string[] {
  const out: string[] = []
  if (caps.webserial) out.push('Web Serial')
  if (caps.webusb) out.push('WebUSB')
  if (caps.wifi_mqtt) out.push('Wi-Fi MQTT')
  if (caps.encoder) out.push('EC11 encoder')
  if (caps.touch) out.push('Touch')
  return out
}
