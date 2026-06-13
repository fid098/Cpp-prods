# 01 — CLI PnL Calculator

> Phase 1 · Fundamentals | C++23 | Realized & unrealized profit/loss

## What You'll Build

A command-line tool that tracks stock positions and computes **realized** and **unrealized PnL** in real time. The user enters trades interactively; the program updates positions and prints PnL after each entry.

```
PnL Calculator — Commands: BUY <sym> <qty> <price> | SELL <sym> <qty> <price> | PRICE <sym> <price> | LIST | QUIT

> BUY AAPL 100 150.00
  AAPL  qty=100  avg_cost=150.00  unrealized_pnl=0.00

> BUY AAPL 50 160.00
  AAPL  qty=150  avg_cost=153.33  unrealized_pnl=0.00

> PRICE AAPL 170.00
  AAPL  qty=150  avg_cost=153.33  unrealized_pnl=2500.50

> SELL AAPL 50 175.00
  Realized PnL: +1083.50
  AAPL  qty=100  avg_cost=153.33  unrealized_pnl=1667.00

> QUIT
Total Realized PnL: +1083.50
```

## Learning Objectives

- Declare and use primitive types: `int`, `double`, `std::string`, `bool`
- Write named functions with typed parameters and return values
- Use arithmetic operators: `+`, `-`, `*`, `/`
- Read input with `std::cin`, print output with `std::cout`
- Understand weighted average cost calculation
- Use `std::unordered_map` to store a collection of positions

## Project Structure

```
01-cli-pnl-calculator/
├── include/
│   └── pnl_calculator.hpp    ← pure logic functions (implement here)
├── src/
│   └── main.cpp              ← CLI loop (implement here)
├── tests/
│   └── test_pnl.cpp          ← test suite (run to verify your work)
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. **BUY** — add a buy trade; update the position using weighted average cost
2. **SELL** — close part or all of a position; compute and accumulate realized PnL; reject if selling more than you own
3. **PRICE** — set the current market price for a symbol (used for unrealized PnL)
4. **LIST** — print all open positions with their unrealized PnL
5. **QUIT** — print total realized PnL and exit cleanly
6. Print a clear error message for unknown symbols or invalid quantities; do not crash

## Formulas

```
Weighted average cost  = (old_qty * old_avg + new_qty * price) / (old_qty + new_qty)
Realized PnL (sell)    = (sell_price − avg_cost) × qty_sold
Unrealized PnL         = (current_price − avg_cost) × qty_held
```

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/pnl_calculator          # Windows: .\build\Debug\pnl_calculator.exe
```

## Run Tests

```bash
cd build
ctest --output-on-failure
```

## Key Resources

| Concept | Link |
|---------|------|
| Variables and fundamental types | https://en.cppreference.com/w/cpp/language/types |
| Arithmetic operators | https://en.cppreference.com/w/cpp/language/operator_arithmetic |
| Functions | https://en.cppreference.com/w/cpp/language/functions |
| std::cin / std::cout | https://en.cppreference.com/w/cpp/io/cin |
| std::string | https://en.cppreference.com/w/cpp/string/basic_string |
| std::unordered_map | https://en.cppreference.com/w/cpp/container/unordered_map |
| std::fixed / std::setprecision | https://en.cppreference.com/w/cpp/io/manip/fixed |

## Stretch Goals

- Add a `HISTORY` command that prints every trade in chronological order
- Support multiple symbols at once (the skeleton already uses `unordered_map`)
- Add input validation for non-numeric price and quantity
- Export a CSV trade log on QUIT
