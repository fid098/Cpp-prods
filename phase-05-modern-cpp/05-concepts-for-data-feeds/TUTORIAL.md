# Tutorial — Concepts for Data Feeds

## Step 1 — Why Concepts?

Without concepts:
```cpp
template<typename T>
void run_feed(T& feed) { feed.next(); }

run_feed(42);  // Error: "int has no member 'next'" — confusing message
```

With a concept:
```cpp
template<DataFeed T>
void run_feed(T& feed) { feed.next(); }

run_feed(42);  // Error: "int does not satisfy DataFeed" — clear!
```

---

## Step 2 — Defining a Concept

```cpp
#include <concepts>

template<typename T>
concept DataFeed = requires(T feed) {
    { feed.next()  } -> std::same_as<bool>;          // next() returns bool
    { feed.price() } -> std::convertible_to<double>; // price() returns something double-ish
    { feed.symbol()} -> std::convertible_to<std::string>;
};
```

A `requires` expression lists the valid operations on `T`. If any operation is missing or returns the wrong type, the concept is not satisfied.

> **Docs:** https://en.cppreference.com/w/cpp/language/requires

---

## Step 3 — Constraining a Function Template

```cpp
// Verbose syntax:
template<typename T>
    requires DataFeed<T>
void run(T& feed) { while (feed.next()) process(feed.price()); }

// Shorthand (C++20):
template<DataFeed T>
void run(T& feed) { while (feed.next()) process(feed.price()); }

// Abbreviated (C++20 concept function parameter):
void run(DataFeed auto& feed) { while (feed.next()) process(feed.price()); }
```

All three forms are equivalent.

---

## Step 4 — Implementing a Concrete Feed

```cpp
class TickFeed {
public:
    explicit TickFeed(std::vector<double> prices)
        : prices_{std::move(prices)}, idx_{0} {}

    bool        next()   { return idx_++ < static_cast<int>(prices_.size()); }
    double      price()  const { return prices_[idx_ - 1]; }
    std::string symbol() const { return "FEED"; }

private:
    std::vector<double> prices_;
    int idx_{0};
};

static_assert(DataFeed<TickFeed>);  // verify at compile time
```

---

## Step 5 — Strategy Concept

```cpp
template<typename T>
concept TradingStrategy = requires(T strat, double price, int bar) {
    { strat.signal(price, bar) } -> std::same_as<int>;
    { strat.reset()            } -> std::same_as<void>;
};
```

Then compose them:
```cpp
template<DataFeed Feed, TradingStrategy Strat>
double run_backtest(Feed feed, Strat strategy) { /* ... */ }
```

> **Docs:** https://en.cppreference.com/w/cpp/language/constraints
