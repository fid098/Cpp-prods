# Tutorial — Final Quant Platform

## Architecture Overview

The platform is built around three layers:

1. **Data layer** — `PriceStore` (atomics) + `OrderBook` (sorted maps)
2. **Pricing layer** — `BSM`, `Binomial`, `Stats<T>`
3. **Application layer** — `Portfolio`, `RiskEngine`, `Backtester`, CLI

All subsystems communicate through well-defined interfaces. No global state; everything is injected via constructor.

---

## Design Decisions

### Why `std::unique_ptr` everywhere?

```cpp
class Platform {
    std::unique_ptr<PriceStore>  prices_;
    std::unique_ptr<OrderBook>   book_;
    std::unique_ptr<Portfolio>   portfolio_;
    std::unique_ptr<RiskEngine>  risk_;
    std::unique_ptr<Backtester>  backtester_;
};
```

`unique_ptr` makes ownership crystal-clear: `Platform` owns everything, subsystems don't outlive it. No `delete`, no leaks.

### Why `std::variant` for events?

The backtester's event bus uses `std::variant<BarEvent, SignalEvent, OrderEvent, FillEvent>`. This is type-safe (no `void*`), exhaustive (compiler warns on unhandled types), and zero-overhead compared to virtual dispatch.

### Why templates for statistics?

`Stats<T>` works for `float`, `double`, and `long double`. The financial math precision ladder: use `double` for most computations, `long double` for Monte Carlo accumulators.

---

## Step 1 — Wiring the Platform

```cpp
class Platform {
public:
    Platform() {
        prices_     = std::make_unique<PriceStore>();
        book_       = std::make_unique<OrderBook>();
        portfolio_  = std::make_unique<Portfolio>(1'000'000.0);
        risk_engine_= std::make_unique<RiskEngine>();
    }

    void start_feed() {
        feed_thread_ = std::jthread([this](std::stop_token st) {
            run_feed(st);
        });
    }
    // ...
};
```

---

## Step 2 — C++23 Features Used

### `std::expected` (C++23) — error handling without exceptions

```cpp
#include <expected>
std::expected<double, std::string> price_option(const OptionParams& p) {
    if (p.T <= 0) return std::unexpected{"time to expiry must be positive"};
    return bsm(p.S, p.K, p.r, p.sigma, p.T).call;
}
```

### Structured bindings in for loops

```cpp
for (auto& [symbol, position] : portfolio.positions()) {
    std::cout << symbol << ": " << position.qty << " @ " << position.avg_cost << "\n";
}
```

### `std::ranges` + views (C++20/23)

```cpp
auto positive_returns = returns
    | std::views::filter([](double r) { return r > 0.0; });
double gross_profit = std::ranges::fold_left(positive_returns, 0.0, std::plus<>{});
```

> **Docs:** https://en.cppreference.com/w/cpp/ranges

---

## Step 3 — The Full Test Suite Structure

The test suite for this capstone uses every GTest feature:
- `TEST_F` fixtures for Platform setup/teardown
- `TEST_P` parameterised tests for option pricing across strikes
- `MOCK_METHOD` to mock the market data feed in backtester tests
- `EXPECT_THAT` with matchers for portfolio assertions

---

## Step 4 — Building for Release

```bash
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release --parallel
```

For sanitizer-checked debug:
```bash
cmake -S . -B build/san -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -g"
cmake --build build/san
ctest --test-dir build/san -V
```

---

## Congratulations

If you've built this platform, you have demonstrated:
- Memory management without leaks (Phase 2)
- Object-oriented design with proper abstraction (Phase 3)
- Efficient data structures for real-time systems (Phase 4)
- Modern C++ idioms that professional quant devs use every day (Phase 5)
- Concurrency, templates, and exception safety (Phase 6)
- Professional build systems and testing (Phase 7)
- Full system design and integration (Phase 8)

This is a CV-worthy project. Put it on GitHub with a demo GIF.
