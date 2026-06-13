# 02 — Market Regime Classifier

> Phase 1 · Fundamentals | C++23 | Classify daily returns into market regimes

## What You'll Build

A program that reads a sequence of daily percentage returns and labels each one as **Bull**, **Bear**, **Volatile**, or **Flat** using a set of simple threshold rules. It then prints a summary of how many days belonged to each regime.

```
Market Regime Classifier
Enter daily returns (% change), one per line. Enter 'done' to finish.

> 1.5
  Day 1:  +1.50%  →  BULL
> -2.1
  Day 2:  -2.10%  →  BEAR
> 3.2
  Day 3:  +3.20%  →  VOLATILE
> 0.2
  Day 4:  +0.20%  →  FLAT
> done

=== Summary ===
  Bull:     1 day(s)
  Bear:     1 day(s)
  Volatile: 1 day(s)
  Flat:     1 day(s)
  Total:    4 day(s)
```

## Learning Objectives

- Use `if / else if / else` for multi-branch logic
- Understand comparison operators: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Use logical operators: `&&`, `||`, `!`
- Define and use C++ `enum class` for named constants
- Use `switch` on an enum value for clean dispatch
- Understand `std::abs` for absolute values

## Project Structure

```
02-market-regime-classifier/
├── include/
│   └── regime_classifier.hpp   ← Regime enum + classify() function
├── src/
│   └── main.cpp                ← input loop and summary
├── tests/
│   └── test_regime.cpp         ← test suite
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Classification Rules

| Regime   | Condition                                |
|----------|------------------------------------------|
| VOLATILE | `std::abs(return) > 2.0%`               |
| BULL     | `return >= 1.0%` (and not volatile)      |
| BEAR     | `return <= -1.0%` (and not volatile)     |
| FLAT     | `-1.0% < return < 1.0%`                  |

Apply them in this order: VOLATILE check first, then BULL/BEAR, then FLAT as the default.

## Requirements

1. Implement `classify(double return_pct) → Regime` in the header
2. Accept returns from `std::cin` until the user types `done`
3. Print the regime label for each day
4. Print a summary count for each regime at the end

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/regime_classifier
```

## Run Tests

```bash
cd build && ctest --output-on-failure
```

## Key Resources

| Concept | Link |
|---------|------|
| if / else | https://en.cppreference.com/w/cpp/language/if |
| Comparison operators | https://en.cppreference.com/w/cpp/language/operator_comparison |
| Logical operators | https://en.cppreference.com/w/cpp/language/operator_logical |
| enum class | https://en.cppreference.com/w/cpp/language/enum |
| switch | https://en.cppreference.com/w/cpp/language/switch |
| std::abs | https://en.cppreference.com/w/cpp/numeric/math/abs |

## Stretch Goals

- Add a `CRASH` regime for returns worse than −5% and a `RALLY` regime for gains above +5%
- Compute and print the average return per regime
- Detect and report "volatility clusters" (3+ consecutive VOLATILE days)
