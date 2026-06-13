# 02 — Options Pricing Library

## What You'll Build

A complete **C++ options pricing library** covering three pricing models and a full set of Greeks. This is the kind of tool used by quant desks at banks and hedge funds every day.

## Models Implemented

| Model | Options | Notes |
|-------|---------|-------|
| Black-Scholes-Merton | European calls and puts | Closed-form, exact |
| Binomial tree (CRR) | European and American | Supports early exercise |
| Monte Carlo | European + path-dependent | Parallelised with `std::async` |

## Greeks Implemented

| Greek | Meaning | Method |
|-------|---------|--------|
| Delta | Price sensitivity to spot | Analytical (BSM) |
| Gamma | Delta sensitivity to spot | Analytical (BSM) |
| Theta | Price sensitivity to time | Analytical (BSM) |
| Vega | Price sensitivity to vol | Analytical (BSM) |
| Rho | Price sensitivity to rate | Analytical (BSM) |
| Implied vol | Vol that gives observed price | Newton-Raphson on BSM |

## Key Resources

| Topic | Link |
|-------|------|
| Black-Scholes formula | https://en.wikipedia.org/wiki/Black-Scholes_model |
| `std::erfc` for N(x) | https://en.cppreference.com/w/cpp/numeric/math/erfc |
| Binomial tree (CRR) | https://en.wikipedia.org/wiki/Binomial_options_pricing_model |
| Newton-Raphson | https://en.wikipedia.org/wiki/Newton%27s_method |
| `std::async` | https://en.cppreference.com/w/cpp/thread/async |

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/options_pricer
ctest --test-dir build -V
```
