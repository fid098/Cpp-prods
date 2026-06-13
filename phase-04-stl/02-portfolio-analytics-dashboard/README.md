# 02 — Portfolio Analytics Dashboard

> Phase 4 · STL | C++23 | Portfolio weights, exposure, concentration, and returns

## What You'll Build

A command-line dashboard that loads a portfolio from a `std::vector` and computes: position weights, sector exposure, concentration (Herfindahl index), and period returns.

```
=== Portfolio Analytics Dashboard ===

Symbol   Qty    Price     Value      Weight
AAPL     100    175.00   17500.00   30.1%
MSFT     200    290.00   58000.00   ... 
...

Sector Exposure:
  Tech:     75.2%
  Finance:  24.8%

Herfindahl Index (concentration): 0.28  (diversified < 0.25, concentrated > 0.25)
```

## Learning Objectives

- Store portfolio data in `std::vector<Holding>`
- Use `std::accumulate` to compute totals
- Use `std::sort` with a lambda comparator
- Use `std::transform` to produce a new vector from an existing one
- Use `std::max_element` / `std::min_element`
- Use `std::find_if` to search

## Key Resources

| Concept | Link |
|---------|------|
| std::vector | https://en.cppreference.com/w/cpp/container/vector |
| std::accumulate | https://en.cppreference.com/w/cpp/algorithm/accumulate |
| std::sort | https://en.cppreference.com/w/cpp/algorithm/sort |
| std::transform | https://en.cppreference.com/w/cpp/algorithm/transform |
| std::max_element | https://en.cppreference.com/w/cpp/algorithm/max_element |
| std::find_if | https://en.cppreference.com/w/cpp/algorithm/find |
