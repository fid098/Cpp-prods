# Tutorial — Portfolio & Position Model

## Step 1 — The Position Class

```cpp
class Position {
public:
    Position(std::string symbol, int qty, double price)
        : symbol_{std::move(symbol)}, qty_{qty}, price_{price} {}

    // Compiler-generated copy constructor is fine here (no raw pointers)
    double      value()  const { return static_cast<double>(qty_) * price_; }
    std::string symbol() const { return symbol_; }
    int         qty()    const { return qty_; }
    double      price()  const { return price_; }
    void set_price(double p)   { price_ = p; }

private:
    std::string symbol_;
    int         qty_;
    double      price_;
};
```

Because `Position` only contains `std::string`, `int`, and `double` — types that copy themselves correctly — the **compiler-generated copy constructor works fine**. You only need to write one explicitly when you have raw pointers or resources that need deep copying.

> **Docs:** https://en.cppreference.com/w/cpp/language/copy_constructor

---

## Step 2 — The Portfolio Class

```cpp
#include <vector>
#include <stdexcept>

class Portfolio {
public:
    void add_position(const Position& pos) {
        positions_.push_back(pos);
    }

    double total_value() const {
        double total = 0.0;
        for (const auto& p : positions_) total += p.value();
        return total;
    }

    double weight(const std::string& symbol) const {
        double tv = total_value();
        if (tv == 0.0) return 0.0;
        for (const auto& p : positions_) {
            if (p.symbol() == symbol) return p.value() / tv;
        }
        return 0.0;
    }

    // ...
private:
    std::vector<Position> positions_;
};
```

`std::vector<Position>` stores `Position` objects by value and deep-copies them automatically when the `Portfolio` is copied.

---

## Step 3 — Copy Semantics Demonstration

```cpp
Portfolio original;
original.add_position({"AAPL", 100, 175.0});

Portfolio copy = original;         // copy constructor (compiler generated)
copy.positions_[0].set_price(200.0); // modify the copy

// The original is unchanged:
assert(original.total_value() == 17500.0);  // still 175 * 100
```

This works because `std::vector`'s copy constructor deep-copies its elements.

---

## Step 4 — Write a Custom Copy Constructor (Practice)

Even though you don't need one for `Portfolio`, practice writing one:

```cpp
Portfolio(const Portfolio& other)
    : positions_{other.positions_}  // vector copies itself
{}

Portfolio& operator=(const Portfolio& other) {
    if (this != &other)
        positions_ = other.positions_;
    return *this;
}
```

> **Docs:** https://en.cppreference.com/w/cpp/language/copy_assignment

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Storing `Position*` in the vector and expecting deep copy | Store `Position` by value |
| Not returning `*this` from `operator=` | Required for chained assignment `a = b = c` |
