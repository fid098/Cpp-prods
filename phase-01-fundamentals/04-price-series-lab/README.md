# 04 — Price Series Lab

> Phase 1 · Fundamentals | C++23 | Analyse arrays of prices: returns, drawdown, highs, lows

## What You'll Build

A analytics engine for a fixed series of historical prices. Given an array of closing prices, compute daily returns, the all-time high, all-time low, and the maximum drawdown.

```
=== Price Series Analysis ===
Prices: 100.0  102.5  101.0  105.0  98.0  107.0  103.5

Daily returns (%):
  Day 1→2:  +2.50%
  Day 2→3:  -1.46%
  Day 3→4:  +3.96%
  Day 4→5:  -6.67%
  Day 5→6:  +9.18%
  Day 6→7:  -3.27%

Statistics:
  High:            107.00
  Low:              98.00
  Max Drawdown:     -6.67%   (peak: 105.00  trough: 98.00)
  Average Return:  +0.71%
```

## Learning Objectives

- Declare and index into C-style arrays and `std::array`
- Write loops: `for` (index-based), range-for
- Compute min, max, and averages using loops
- Understand what a drawdown is and how to detect peak-to-trough
- Use `std::array` from `<array>`

## Project Structure

```
04-price-series-lab/
├── include/
│   └── price_series.hpp    ← analysis functions
├── src/
│   └── main.cpp            ← runs analysis on a hardcoded series
├── tests/
│   └── test_price_series.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. `daily_return(prev, curr)` → `(curr - prev) / prev * 100.0`
2. `series_high(prices, n)` → highest price in the array
3. `series_low(prices, n)` → lowest price in the array
4. `max_drawdown(prices, n)` → worst peak-to-trough decline as a percentage
5. `average_return(prices, n)` → mean of all daily returns
6. Print all results in a formatted table

The functions accept a `const double* prices` pointer and a `size_t n` count — this teaches you that arrays decay to pointers.

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/price_series_lab
```

## Run Tests

```bash
cd build && ctest --output-on-failure
```

## Key Resources

| Concept | Link |
|---------|------|
| Arrays | https://en.cppreference.com/w/cpp/language/array |
| std::array | https://en.cppreference.com/w/cpp/container/array |
| for loops | https://en.cppreference.com/w/cpp/language/for |
| Range-for | https://en.cppreference.com/w/cpp/language/range-for |
| std::numeric_limits | https://en.cppreference.com/w/cpp/types/numeric_limits |

## Stretch Goals

- Detect the longest consecutive up-run (days where each day's return is positive)
- Compute the Sharpe-like ratio: `average_return / std_dev_return`
- Accept a price series from a file or stdin instead of a hardcoded array
