# 03 — Trade Ticket Validator

> Phase 1 · Fundamentals | C++23 | Validate order tickets before execution

## What You'll Build

A validator that checks incoming order tickets against a set of business rules and either accepts them or rejects them with a specific reason. This mirrors the pre-trade validation layer in every real trading system.

```
Trade Ticket Validator
Format: <symbol> <side> <qty> <type> <price>
  side: BUY | SELL
  type: MARKET | LIMIT

> AAPL BUY 100 LIMIT 150.50
  ACCEPTED

> AAPL BUY -5 LIMIT 150.50
  REJECTED: quantity must be positive

>  BUY 100 LIMIT 150.50
  REJECTED: symbol cannot be empty

> AAPL BUY 100 LIMIT 0.00
  REJECTED: limit price must be positive

> AAPL SELL 100 MARKET 0.00
  ACCEPTED  (market orders ignore the price field)

> quit
```

## Learning Objectives

- Work with `std::string`: check length, compare, convert case
- Write multiple overloads of the same function name
- Return `bool` from validation functions
- Use `struct` to group a validation result (accepted/rejected + reason)
- Combine boolean conditions with `&&` and `||`

## Project Structure

```
03-trade-ticket-validator/
├── include/
│   └── trade_validator.hpp    ← TradeTicket struct + validation functions
├── src/
│   └── main.cpp               ← input loop
├── tests/
│   └── test_validator.cpp     ← test suite
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Validation Rules

| Rule | Condition |
|------|-----------|
| Symbol not empty | `symbol.empty() == false` |
| Symbol max length | `symbol.length() <= 5` |
| Valid side | `side == "BUY"` or `side == "SELL"` |
| Positive quantity | `qty > 0` |
| Valid order type | `type == "MARKET"` or `type == "LIMIT"` |
| Limit price positive | if LIMIT order: `price > 0.0` |

## Requirements

1. Define a `TradeTicket` struct with fields: `symbol`, `side`, `qty`, `type`, `price`
2. Define a `ValidationResult` struct with `bool accepted` and `std::string reason`
3. Implement `validate(const TradeTicket&) → ValidationResult`
4. Implement individual rule functions such as `is_valid_symbol(std::string)` and `is_valid_qty(int)` — these are the overloads you practice
5. Read tickets from `std::cin`; print ACCEPTED or REJECTED with reason

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/trade_validator
```

## Run Tests

```bash
cd build && ctest --output-on-failure
```

## Key Resources

| Concept | Link |
|---------|------|
| std::string methods | https://en.cppreference.com/w/cpp/string/basic_string |
| string::empty() | https://en.cppreference.com/w/cpp/string/basic_string/empty |
| string::length() | https://en.cppreference.com/w/cpp/string/basic_string/size |
| Function overloading | https://en.cppreference.com/w/cpp/language/overload_resolution |
| bool type | https://en.cppreference.com/w/cpp/language/types |

## Stretch Goals

- Add a `CANCEL` order type
- Check that the symbol matches a known universe (hardcode a small `std::array` of valid tickers)
- Add a `notional_limit` rule: reject if `qty * price > 1,000,000`
