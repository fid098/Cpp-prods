# Tutorial — Trade Ticket Validator

## Step 1 — Define Your Data Types

Open `include/trade_validator.hpp`. Start by defining the two structs:

```cpp
#pragma once
#include <string>

struct TradeTicket {
    std::string symbol;
    std::string side;    // "BUY" or "SELL"
    int         qty{0};
    std::string type;    // "MARKET" or "LIMIT"
    double      price{0.0};
};

struct ValidationResult {
    bool        accepted{false};
    std::string reason;   // empty if accepted
};
```

> **Docs:** https://en.cppreference.com/w/cpp/language/struct

---

## Step 2 — Individual Rule Functions

Each rule becomes its own function. This makes them unit-testable and easy to compose.

```cpp
// Rule: symbol must be 1–5 characters
inline bool is_valid_symbol(const std::string& symbol) {
    // TODO: return !symbol.empty() && symbol.length() <= 5
    return false;
}

// Rule: side must be "BUY" or "SELL"
inline bool is_valid_side(const std::string& side) {
    // TODO: return side == "BUY" || side == "SELL"
    return false;
}

// Rule: quantity must be strictly positive
inline bool is_valid_qty(int qty) {
    // TODO: return qty > 0
    return false;
}

// Rule: order type must be "MARKET" or "LIMIT"
inline bool is_valid_type(const std::string& type) {
    return false; // TODO
}

// Rule: LIMIT orders must have a positive price; MARKET orders ignore price
inline bool is_valid_price(const std::string& type, double price) {
    // TODO: if type == "LIMIT", price must be > 0.0
    //       if type == "MARKET", always valid
    return false;
}
```

Notice `is_valid_price` takes two parameters: the order type and the price. That is not overloading — it is just a two-parameter function. A real overload example:

```cpp
// Overload 1: check a raw integer quantity
inline bool is_valid_qty(int qty)           { return qty > 0; }

// Overload 2: check a quantity already stored as double (e.g. from CSV parsing)
inline bool is_valid_qty(double qty)        { return qty > 0.0 && qty == static_cast<int>(qty); }
```

The compiler picks the right one based on the argument type.

> **Docs:** https://en.cppreference.com/w/cpp/language/overload_resolution

---

## Step 3 — The Master validate() Function

```cpp
inline ValidationResult validate(const TradeTicket& t) {
    if (!is_valid_symbol(t.symbol))
        return {false, "symbol cannot be empty or longer than 5 characters"};

    if (!is_valid_side(t.side))
        return {false, "side must be BUY or SELL"};

    if (!is_valid_qty(t.qty))
        return {false, "quantity must be positive"};

    if (!is_valid_type(t.type))
        return {false, "type must be MARKET or LIMIT"};

    if (!is_valid_price(t.type, t.price))
        return {false, "limit price must be positive"};

    return {true, ""};
}
```

The `return {false, "..."}` syntax is **aggregate initialisation** — it fills the struct fields in order.

> **Docs:** https://en.cppreference.com/w/cpp/language/aggregate_initialization

---

## Step 4 — Input Loop in main.cpp

```cpp
#include <iostream>
#include <string>
#include "trade_validator.hpp"

int main() {
    std::cout << "Trade Ticket Validator\n";
    std::cout << "Format: <symbol> <side> <qty> <type> <price>\n\n";

    TradeTicket t;
    while (std::cin >> t.symbol) {
        if (t.symbol == "quit") break;

        std::cin >> t.side >> t.qty >> t.type >> t.price;

        ValidationResult result = validate(t);

        if (result.accepted) {
            std::cout << "  ACCEPTED\n\n";
        } else {
            std::cout << "  REJECTED: " << result.reason << "\n\n";
        }
    }
    return 0;
}
```

---

## Step 5 — std::string Key Operations

You need these string operations for the validators:

```cpp
std::string s = "AAPL";

s.empty()             // true if length is 0
s.length()            // number of characters
s == "BUY"            // exact comparison (case-sensitive)
s.size()              // same as length()
```

> **Docs:** https://en.cppreference.com/w/cpp/string/basic_string

---

## Step 6 — Run the Tests

```bash
cmake -S . -B build && cmake --build build
cd build && ctest --output-on-failure
```

The test suite checks each rule function independently plus the combined `validate()` function.

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| `s == 'A'` uses a `char` literal | Use double quotes: `s == "BUY"` |
| Checking `price > 0` even for MARKET orders | Gate the price check on `type == "LIMIT"` |
| Returning `{true}` without initialising `reason` | `reason` defaults to `""` thanks to the struct default |
