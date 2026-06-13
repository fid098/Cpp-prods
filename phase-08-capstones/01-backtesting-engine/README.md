# 01 — Backtesting Engine

## What You'll Build

An **event-driven backtesting framework** that replays historical price data, executes a configurable trading strategy, and produces professional performance metrics. You'll implement the full pipeline: data ingestion → signal generation → order execution simulation → PnL tracking → reporting.

## Architecture

```
CSV Data  →  BarFeed  →  Strategy  →  Broker  →  Portfolio
                ↓                          ↓
           BarEvent                  FillEvent
```

Events flow through the system. The `Strategy` receives `BarEvent`s and emits `SignalEvent`s. The `Broker` converts signals to `FillEvent`s (simulating slippage and commission). The `Portfolio` tracks positions and equity curve.

## Learning Objectives

| C++ Concept | Application |
|-------------|-------------|
| `std::variant` + `std::visit` | Type-safe event bus |
| `std::unique_ptr` | Strategy/broker ownership |
| Template method pattern | Pluggable strategies |
| `std::deque<Event>` | Event queue |
| STL algorithms (`std::transform`, `std::accumulate`) | Performance metric computation |
| RAII CSV loader (from Phase 6) | Data ingestion |
| `std::chrono` | Timestamps |

## Performance Metrics Computed

| Metric | Formula |
|--------|---------|
| Total return | `(final_equity - initial_equity) / initial_equity` |
| Annualised Sharpe | `mean(daily_returns) / std(daily_returns) * sqrt(252)` |
| Max drawdown | `max(peak - trough) / peak` |
| Win rate | `winning_trades / total_trades` |
| Profit factor | `gross_profit / gross_loss` |

## Key Resources

| Topic | Link |
|-------|------|
| `std::variant` | https://en.cppreference.com/w/cpp/utility/variant |
| `std::visit` | https://en.cppreference.com/w/cpp/utility/variant/visit |
| `std::deque` | https://en.cppreference.com/w/cpp/container/deque |
| `std::unique_ptr` | https://en.cppreference.com/w/cpp/memory/unique_ptr |
| Event-driven architecture | https://en.wikipedia.org/wiki/Event-driven_architecture |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/backtester data/sample_ohlcv.csv
ctest --test-dir build -V
```
