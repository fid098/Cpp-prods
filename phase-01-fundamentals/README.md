# Phase 1 — Fundamentals

**C++ concepts covered:** types, variables, arithmetic, functions, I/O, conditionals, enums, strings, arrays, loops

Phase 1 establishes the bedrock. You will write standalone programs that compile, run, and solve real finance problems using only the core language — no classes, no templates, no standard containers beyond `std::array` and `std::string`.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [cli-pnl-calculator](01-cli-pnl-calculator/) | Realized/unrealized PnL from a trade stream | variables, arithmetic, functions |
| 02 | [market-regime-classifier](02-market-regime-classifier/) | Classify daily returns: bull/bear/volatile/flat | conditionals, enums, switch |
| 03 | [trade-ticket-validator](03-trade-ticket-validator/) | Validate order tickets, explain rejections | strings, function overloads, booleans |
| 04 | [price-series-lab](04-price-series-lab/) | Returns, max drawdown, highs and lows | arrays, std::array, loops |

## Learning Path

Work through the projects in order. Each one introduces new syntax and re-uses everything you have already learned.

## How to Build Any Project

```bash
cd 01-cli-pnl-calculator          # or any other project folder
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/<executable_name>
```

## Phase Resources

- https://en.cppreference.com/w/cpp/language/types
- https://en.cppreference.com/w/cpp/language/functions
- https://en.cppreference.com/w/cpp/language/statements
- https://en.cppreference.com/w/cpp/string/basic_string
- https://en.cppreference.com/w/cpp/container/array
