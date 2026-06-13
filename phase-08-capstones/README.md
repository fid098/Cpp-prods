# Phase 8 — Capstones

**C++ concepts covered:** full system design, all prior phases combined, end-to-end C++23

Phase 8 is the boss level. Each project is a complete, portfolio-grade system. The final project — **05-final-quant-platform** — integrates everything you have built across the entire curriculum into one production-quality application.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [backtesting-engine](01-backtesting-engine/) | Event-driven backtest with metrics | system design, STL, testing |
| 02 | [options-pricing-library](02-options-pricing-library/) | Black-Scholes + Monte Carlo pricer | math, templates, performance |
| 03 | [market-data-terminal](03-market-data-terminal/) | CLI terminal with watchlists and analytics | architecture, file I/O, UI loops |
| 04 | [risk-and-var-engine](04-risk-and-var-engine/) | VaR, stress tests, exposure reports | statistics, concurrency, reports |
| 05 | [final-quant-platform](05-final-quant-platform/) | Everything combined in one platform | end-to-end C++23 |

## What Makes a Good Capstone

- Clean CMake build that a stranger can clone and build in under two minutes
- A `README.md` that explains what the system does and how to run it
- A comprehensive test suite (all tests pass)
- No memory leaks (run under ASan)
- A brief `DESIGN.md` that explains the key architectural decisions

## Phase Resources

All previous phase resources apply. Additionally:
- https://en.cppreference.com/w/cpp/chrono (for performance timing)
- https://en.wikipedia.org/wiki/Value_at_risk (VaR concepts)
- https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model (options pricing)
