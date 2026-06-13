# 04 — Atomic Market Data Cache

## What You'll Build

A **lock-free, thread-safe market data cache** that stores the latest bid, ask, and last-trade prices for a set of instruments. A publisher thread continuously updates prices; multiple subscriber threads read them — all without a single mutex.

You'll implement a `MarketDataCache` class using `std::atomic`, learn memory ordering semantics, and benchmark the lock-free design against a mutex-protected equivalent.

## Learning Objectives

| C++ Topic | Finance Application |
|-----------|---------------------|
| `std::atomic<T>` | Thread-safe price updates |
| `memory_order_relaxed` / `acquire` / `release` | Publication protocol for price ticks |
| `std::atomic_flag` | Spinning lock primitive |
| `std::atomic<double>` | Concurrent bid/ask cache |
| Compare-and-swap (`compare_exchange_weak`) | Optimistic price update |
| `std::jthread` (C++20) | Auto-joining publisher thread |

## Project Structure

```
04-atomic-market-data-cache/
├── README.md
├── TUTORIAL.md
├── CMakeLists.txt
├── include/
│   └── atomic_cache.hpp
├── src/
│   └── main.cpp
└── tests/
    └── test_atomic_cache.cpp
```

## Requirements

1. `PriceTick` stores bid, ask, last as `std::atomic<double>`.
2. `MarketDataCache` holds a fixed-size array of `PriceTick` (max 16 symbols).
3. `update(symbol, bid, ask, last)` stores prices with `memory_order_release`.
4. `read(symbol)` returns a snapshot with `memory_order_acquire`.
5. `update_count()` returns total updates atomically with `memory_order_relaxed`.
6. Demonstrate that multiple threads can read and write concurrently without data races (verified with ThreadSanitizer).

## Key Resources

| Topic | Link |
|-------|------|
| `std::atomic` | https://en.cppreference.com/w/cpp/atomic/atomic |
| Memory order | https://en.cppreference.com/w/cpp/atomic/memory_order |
| `atomic_flag` | https://en.cppreference.com/w/cpp/atomic/atomic_flag |
| `compare_exchange_weak` | https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange |
| `std::jthread` | https://en.cppreference.com/w/cpp/thread/jthread |
| Lock-free data structures | https://en.cppreference.com/w/cpp/atomic |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/atomic_cache        # publisher + 4 reader threads demo
ctest --test-dir build -V   # run tests
```
