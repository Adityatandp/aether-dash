# aether-host

Local **real PC metrics** agent for Aether Dash.

Use this when you don’t have ESP32 hardware yet, but want the web dashboard to show **real** CPU/RAM/GPU data instead of the dummy simulator.

```text
Windows/macOS/Linux PC
  └─ aether-host  (localhost:8787)
        └─ aether-web-app polls /metrics @ ~10 Hz
```

## Quick start

```bash
cd aether-host
npm install
npm start
```

Health check: [http://127.0.0.1:8787/health](http://127.0.0.1:8787/health)  
Metrics snapshot: [http://127.0.0.1:8787/metrics](http://127.0.0.1:8787/metrics)

Then in another terminal:

```bash
cd aether-web-app
npm run dev
```

Open the app → click **Use real PC metrics**.

## What it collects

| Field | Source |
|---|---|
| `cpu_pct` | `systeminformation` current load |
| `ram_pct` | active/total memory |
| `gpu_pct` | `nvidia-smi` (NVIDIA) or best-effort graphics API |
| `vram_pct` | `nvidia-smi` memory used/total |
| `gpu_temp_c` | `nvidia-smi` |
| `pkg_watts` | `nvidia-smi` power draw (GPU package proxy) |
| `label` | running Ollama / LM Studio model if detected |
| `tokens_per_s` | Ollama approx from recent `/api/ps` activity when available; else omitted |

## Config (optional env)

| Env | Default | Meaning |
|---|---|---|
| `PORT` | `8787` | HTTP port |
| `OLLAMA_HOST` | `http://127.0.0.1:11434` | Ollama base URL |
| `LMSTUDIO_HOST` | `http://127.0.0.1:1234` | LM Studio OpenAI-compatible base |

## Notes

- Browser cannot read CPU/GPU directly with full fidelity → this tiny local agent is required.
- Same metric payload shape as Aether Protocol v1, so later the web app can forward these frames to ESP32 unchanged.
- Non-NVIDIA GPUs may have limited utilization reporting depending on OS/drivers.
