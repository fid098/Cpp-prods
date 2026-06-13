# 03 — Lambda Strategy Backtester

> Phase 5 · Modern C++ | C++23 | Compose trading rules with lambdas and higher-order functions

## What You'll Build

A simple event-driven backtester where trading strategies are expressed as **lambdas** and composed using higher-order functions. The strategy is a callable that takes a price series and returns buy/sell signals.

```
=== Lambda Strategy Backtester ===
Strategy: momentum (buy if 5-day return > 2%)
Signal history: 0 0 0 0 B 0 0 S 0 B
Final PnL: +$1240.00
```

## Learning Objectives

- Write lambdas with captures by value `[=]`, by reference `[&]`, and mixed `[threshold]`
- Store lambdas in `std::function<Ret(Args...)>`
- Write higher-order functions that accept lambdas as parameters
- Compose strategies: `and_strategy(s1, s2)`, `not_strategy(s)`, `threshold_filter(s, t)`
- Use `auto` for lambda variable types

## Key Resources

| Concept | Link |
|---------|------|
| Lambda expressions | https://en.cppreference.com/w/cpp/language/lambda |
| std::function | https://en.cppreference.com/w/cpp/utility/functional/function |
| Captures | https://en.cppreference.com/w/cpp/language/lambda#Lambda_captures |
| auto keyword | https://en.cppreference.com/w/cpp/language/auto |
