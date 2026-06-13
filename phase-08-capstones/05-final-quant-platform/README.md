# 05 — Final Quant Platform (Capstone)

## What You'll Build

The crown jewel of the curriculum — a **fully integrated quantitative finance platform** combining every C++ concept from all eight phases into a single cohesive system. This is the kind of codebase that appears on senior quant developer CVs.

## Integrated Modules

```
┌─────────────────────────────────────────────────────────┐
│                    Quant Platform CLI                    │
├──────────────┬──────────────┬──────────────┬────────────┤
│ Market Data  │   Order Book │   Options    │    Risk    │
│ Feed (atoms) │ (map/umap)   │ (BSM + Binom)│  (VaR/ES)  │
├──────────────┴──────────────┴──────────────┴────────────┤
│           Portfolio Manager (positions + PnL)           │
├─────────────────────────────────────────────────────────┤
│     Backtester (event-driven, SMA strategy)             │
└─────────────────────────────────────────────────────────┘
```

## C++ Concepts Demonstrated

| Phase | Concepts Used |
|-------|--------------|
| 1 | structs, enums, functions, arrays |
| 2 | RAII, smart pointers, no raw owning pointers |
| 3 | Class hierarchy (`Instrument` → `Equity`, `Option`) |
| 4 | `std::map`, `std::unordered_map`, `std::vector`, STL algorithms |
| 5 | `std::unique_ptr`, move semantics, lambdas, `constexpr`, concepts |
| 6 | Templates (`Stats<T>`), exceptions+RAII, `std::async`, atomics |
| 7 | CMake subdirectories, GoogleTest, sanitizer-clean |
| 8 | System integration, event-driven architecture, `std::variant` |

## CLI Commands

| Command | Description |
|---------|-------------|
| `feed` | Show live simulated market data |
| `book AAPL` | Display AAPL order book |
| `price call AAPL 175 180 0.05 0.25 0.5` | Price a European call |
| `risk` | Run VaR / ES on the current portfolio |
| `backtest` | Run SMA crossover backtest |
| `portfolio` | Show positions and PnL |
| `quit` | Exit |

## Key Resources

| Topic | Link |
|-------|------|
| `std::variant` / `std::visit` | https://en.cppreference.com/w/cpp/utility/variant/visit |
| `std::unique_ptr` | https://en.cppreference.com/w/cpp/memory/unique_ptr |
| `std::jthread` | https://en.cppreference.com/w/cpp/thread/jthread |
| C++23 `std::expected` | https://en.cppreference.com/w/cpp/utility/expected |
| CMake subdirectories | https://cmake.org/cmake/help/latest/command/add_subdirectory.html |
| GTest fixtures | https://google.github.io/googletest/primer.html |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/quant_platform
ctest --test-dir build -V
```
