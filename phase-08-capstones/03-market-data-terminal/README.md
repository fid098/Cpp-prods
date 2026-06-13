# 03 — Market Data Terminal

## What You'll Build

A **live market data terminal** that streams simulated tick data for multiple instruments and renders a refreshing console display — like a mini Bloomberg terminal in your shell. The system uses lock-free atomics for the data store, `std::jthread` for lifecycle management, and ANSI escape codes for the TUI.

## Architecture

```
SimFeed (std::jthread)    ──── tick updates ────►  AtomicPriceStore
                                                          │
                                               (atomic reads, no lock)
                                                          │
Renderer (std::jthread)  ◄──── snapshots ────────────────┘
         │
         ▼
    Console display (ANSI refresh)
```

## Features

- 6 simulated instruments (AAPL, MSFT, TSLA, AMZN, NVDA, GOOGL)
- Live bid/ask/last streaming at ~1000 ticks/sec
- Change indicators (↑ ↓ =) colour-coded
- Spread display in basis points
- Lock-free reads via `std::atomic<double>`

## Learning Objectives

| C++ Feature | Application |
|-------------|-------------|
| `std::atomic<double>` | Lock-free price cache |
| `std::jthread` + `std::stop_token` | Auto-joining threads with cancellation |
| `memory_order_release/acquire` | Publication protocol |
| ANSI escape codes | Console cursor control |
| `std::mt19937` + `std::normal_distribution` | Simulated price movements |
| `std::this_thread::sleep_for` | Rate-limiting the renderer |

## Key Resources

| Topic | Link |
|-------|------|
| `std::jthread` | https://en.cppreference.com/w/cpp/thread/jthread |
| `std::stop_token` | https://en.cppreference.com/w/cpp/thread/stop_token |
| `std::atomic` | https://en.cppreference.com/w/cpp/atomic/atomic |
| ANSI escape codes | https://en.wikipedia.org/wiki/ANSI_escape_code |
| `std::normal_distribution` | https://en.cppreference.com/w/cpp/numeric/random/normal_distribution |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/market_terminal       # runs for 30 seconds then exits
./build/market_terminal 10    # run for 10 seconds
ctest --test-dir build -V
```
