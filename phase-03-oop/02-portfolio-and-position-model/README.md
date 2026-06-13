# 02 — Portfolio & Position Model

> Phase 3 · OOP | C++23 | Build portfolio and position objects with valuation logic

## What You'll Build

A `Position` class and a `Portfolio` class that composes multiple positions, computes portfolio weights, and prints a valuation report.

```
=== Portfolio Valuation ===
AAPL   100 shares @ 175.00   value=17500.00   weight=46.2%
MSFT   200 shares @ 290.00   value=58000.00   weight=40.1% (after copy)
TSLA    50 shares @ 700.00   value=35000.00   weight=...
Total value: $110500.00
```

## Learning Objectives

- Compose objects (Portfolio contains Positions)
- Implement copy constructor and copy assignment operator
- Understand shallow vs deep copy
- Use `std::vector` to store a dynamic collection of objects
- Write member functions that compute derived values

## Key Concepts

| Concept | Link |
|---------|------|
| Composition | https://en.cppreference.com/w/cpp/language/classes |
| Copy constructor | https://en.cppreference.com/w/cpp/language/copy_constructor |
| Copy assignment | https://en.cppreference.com/w/cpp/language/copy_assignment |
| std::vector | https://en.cppreference.com/w/cpp/container/vector |

## Requirements

1. `Position` class: symbol, quantity, price; `value()` returns `qty * price`; copy constructor
2. `Portfolio` class: stores `std::vector<Position>`; `add_position`, `total_value`, `weight(symbol)`, `print_report`
3. Demonstrate that copying a `Portfolio` produces an independent copy (change original → copy unchanged)
4. Tests cover: add, total\_value, weight, copy independence
