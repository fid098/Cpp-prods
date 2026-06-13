# Tutorial — CLI PnL Calculator

## Before You Start

This is a pure fundamentals project. You only need:
- Variables, types, arithmetic
- Functions
- `std::cout` / `std::cin`
- `std::string`
- `std::unordered_map` (follow the examples below — you don't need to understand it fully yet)

If anything is unclear, the docs links in the README are the definitive reference.

---

## Step 1 — Understand the Domain

**Realized PnL** is profit/loss you have locked in by closing a position (selling).

**Unrealized PnL** is the paper profit/loss on shares you still hold.

**Weighted average cost** is how you track what you paid per share across multiple buys:

```
You buy 100 shares @ $150 → avg cost = $150.00
You buy 50 more   @ $160 → avg cost = (100×150 + 50×160) / 150 = $153.33
```

If you then sell 50 shares @ $175:
```
Realized PnL = (175 - 153.33) × 50 = $1083.50
```

---

## Step 2 — Define a Position

Open `include/pnl_calculator.hpp`. A **Position** holds everything we know about one holding:

```cpp
#pragma once
#include <string>

struct Position {
    std::string symbol;
    int         quantity{0};      // shares currently held
    double      avg_cost{0.0};    // weighted average purchase price
    double      current_price{0.0};
};
```

`struct` groups related variables into one named type. The `{0}` and `{0.0}` are default initialisers — the fields start at zero so you never read garbage.

> **Docs:** https://en.cppreference.com/w/cpp/language/struct

---

## Step 3 — Weighted Average Cost Function

Add this function to the header (as `inline` so it can be included in multiple files):

```cpp
// Returns the new average cost after adding `new_qty` shares at `price`
inline double compute_avg_cost(int old_qty, double old_avg,
                                int new_qty, double price) {
    // TODO: implement weighted average formula
    // (old_qty * old_avg + new_qty * price) / (old_qty + new_qty)
    return 0.0; // replace this
}
```

The formula in code:
```cpp
return (static_cast<double>(old_qty) * old_avg + static_cast<double>(new_qty) * price)
       / static_cast<double>(old_qty + new_qty);
```

`static_cast<double>` converts an `int` to `double` before the multiplication to avoid integer division surprises.

> **Docs:** https://en.cppreference.com/w/cpp/language/static_cast

---

## Step 4 — Realized PnL Function

```cpp
// Returns the realized PnL for selling `qty_sold` shares from `pos`
inline double compute_realized_pnl(const Position& pos,
                                    int qty_sold, double sell_price) {
    // TODO: (sell_price - avg_cost) * qty_sold
    return 0.0;
}
```

`const Position&` means "give me the position without copying it, and promise not to modify it". This is the idiomatic way to pass structs to read-only functions.

> **Docs:** https://en.cppreference.com/w/cpp/language/reference

---

## Step 5 — Unrealized PnL Function

```cpp
// Returns the unrealized PnL for the position at its current_price
inline double compute_unrealized_pnl(const Position& pos) {
    // TODO: (current_price - avg_cost) * quantity
    return 0.0;
}
```

---

## Step 6 — Build the CLI Loop

Open `src/main.cpp`. You need a loop that reads commands until the user types QUIT.

```cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "pnl_calculator.hpp"

int main() {
    // A map from symbol string → Position
    std::unordered_map<std::string, Position> positions;
    double total_realized_pnl = 0.0;

    std::cout << "PnL Calculator\n";
    std::cout << "Commands: BUY <sym> <qty> <price>  |  SELL <sym> <qty> <price>\n";
    std::cout << "          PRICE <sym> <price>  |  LIST  |  QUIT\n\n";

    std::string command;
    while (std::cin >> command) {
        if (command == "BUY") {
            std::string sym;  int qty;  double price;
            std::cin >> sym >> qty >> price;
            // TODO: update or create the position
        }
        else if (command == "SELL") {
            std::string sym;  int qty;  double price;
            std::cin >> sym >> qty >> price;
            // TODO: check position exists; compute realized PnL; update position
        }
        else if (command == "PRICE") {
            std::string sym;  double price;
            std::cin >> sym >> price;
            // TODO: update current_price in the position
        }
        else if (command == "LIST") {
            // TODO: iterate and print all positions
        }
        else if (command == "QUIT") {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Total Realized PnL: " << total_realized_pnl << "\n";
            break;
        }
    }
    return 0;
}
```

### Tip — Accessing / Creating a Map Entry

```cpp
// This creates a default Position if "AAPL" doesn't exist yet
Position& pos = positions["AAPL"];
pos.symbol = "AAPL";
```

> **Docs:** https://en.cppreference.com/w/cpp/container/unordered_map/operator_at

---

## Step 7 — Format Output Neatly

Use `std::fixed` and `std::setprecision(2)` to print monetary values:

```cpp
#include <iomanip>
std::cout << std::fixed << std::setprecision(2) << value << "\n";
```

> **Docs:** https://en.cppreference.com/w/cpp/io/manip/fixed

---

## Step 8 — Verify with the Tests

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build && ctest --output-on-failure
```

All three core functions (`compute_avg_cost`, `compute_realized_pnl`, `compute_unrealized_pnl`) are tested. Fix any failures by revisiting the formulas in Steps 3–5.

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| `int / int` gives integer result | Cast to `double` first with `static_cast<double>` |
| Forgetting to update `quantity` after a SELL | Subtract `qty_sold` from `pos.quantity` |
| Printing positions before a PRICE update | `current_price` starts at 0 — that is fine, just document it |
| Reading wrong number of fields | Make sure `std::cin >> sym >> qty >> price` matches the command format |

---

## What's Next

Once all tests pass and the CLI works interactively, move to **Phase 1 · Project 2 — Market Regime Classifier**. That project introduces conditionals, enums, and `switch`.
