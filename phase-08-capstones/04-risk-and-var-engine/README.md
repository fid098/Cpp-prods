# 04 — Risk and VaR Engine

## What You'll Build

A **production-style risk engine** that computes Value-at-Risk (VaR), Expected Shortfall (ES/CVaR), correlation matrices, portfolio beta, drawdown statistics, and concentration metrics for a multi-asset portfolio. Used daily by risk desks at every major financial institution.

## Risk Metrics Implemented

| Metric | Method | Use Case |
|--------|--------|----------|
| VaR (1d, 10d) | Historical simulation | Regulatory capital (Basel III) |
| VaR | Parametric (normal) | Fast approximation |
| VaR | Monte Carlo | Non-normal distributions |
| Expected Shortfall | Historical (tail mean) | More coherent than VaR |
| Correlation matrix | Pearson | Diversification analysis |
| Portfolio beta | OLS vs market | Systematic risk |
| Max drawdown | Peak-to-trough | Strategy evaluation |
| HHI (concentration) | Sum of squared weights | Regulatory compliance |

## Learning Objectives

| C++ Concept | Application |
|-------------|-------------|
| Template statistics (`Stats<T>`) | Generic mean/variance/correlation |
| `std::sort` + percentile | Historical simulation VaR |
| `std::async` | Parallel Monte Carlo VaR |
| `std::vector<std::vector<T>>` | Correlation matrix |
| Structured bindings | Clean loop syntax |
| `std::transform` + `std::inner_product` | Vectorised portfolio return |

## Key Resources

| Topic | Link |
|-------|------|
| Basel III VaR | https://www.bis.org/publ/bcbs128.pdf |
| Expected Shortfall | https://en.wikipedia.org/wiki/Expected_shortfall |
| `std::sort` | https://en.cppreference.com/w/cpp/algorithm/sort |
| `std::inner_product` | https://en.cppreference.com/w/cpp/algorithm/inner_product |
| `std::async` | https://en.cppreference.com/w/cpp/thread/async |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/risk_engine
ctest --test-dir build -V
```
