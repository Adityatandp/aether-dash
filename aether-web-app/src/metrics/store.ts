import type { MetricsPayload } from '../protocol/types'

export type MetricsListener = (metrics: MetricsPayload, hz: number) => void

export class MetricsStore {
  private latest: MetricsPayload = {}
  private timestamps: number[] = []
  private listeners = new Set<MetricsListener>()

  update(metrics: MetricsPayload) {
    this.latest = { ...this.latest, ...metrics }
    const now = Date.now()
    this.timestamps.push(now)
    this.timestamps = this.timestamps.filter((t) => now - t <= 1000)
    const hz = this.timestamps.length
    for (const l of this.listeners) l(this.latest, hz)
  }

  get(): MetricsPayload {
    return this.latest
  }

  subscribe(listener: MetricsListener): () => void {
    this.listeners.add(listener)
    listener(this.latest, this.timestamps.length)
    return () => this.listeners.delete(listener)
  }
}

export const metricsStore = new MetricsStore()
