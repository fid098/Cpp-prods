# Tutorial — Lambda Strategy Backtester

## Step 1 — Lambda Basics

```cpp
auto square = [](double x) { return x * x; };
std::cout << square(5.0);  // 25
```

A lambda is an anonymous function object. The `[]` is the **capture list**, `()` is the parameter list, and `{}` is the body.

> **Docs:** https://en.cppreference.com/w/cpp/language/lambda

---

## Step 2 — Captures

```cpp
double threshold = 0.02;

// Capture by value (copy of threshold)
auto above_threshold = [threshold](double ret) {
    return ret > threshold;
};

// Capture by reference (reads current value every call)
auto above_threshold_ref = [&threshold](double ret) {
    return ret > threshold;
};

threshold = 0.05;
above_threshold(0.03);     // returns true  (captures old 0.02)
above_threshold_ref(0.03); // returns false (reads new 0.05)
```

Rule: capture by value `[=]` for lambdas that outlive the captured variable; by reference `[&]` for short-lived closures.

---

## Step 3 — std::function

Store a lambda (or any callable) in a typed wrapper:

```cpp
#include <functional>

using Signal = int;  // -1=SELL, 0=HOLD, 1=BUY
using Strategy = std::function<Signal(const std::vector<double>&, int)>;

Strategy momentum = [lookback=5, threshold=0.02](
    const std::vector<double>& prices, int i) -> Signal
{
    if (i < lookback) return 0;
    double ret = (prices[i] - prices[i - lookback]) / prices[i - lookback];
    if (ret > threshold) return 1;
    if (ret < -threshold) return -1;
    return 0;
};
```

---

## Step 4 — Higher-Order Functions

```cpp
// Combine two strategies: only signal if BOTH agree
Strategy and_strategy(Strategy s1, Strategy s2) {
    return [s1, s2](const std::vector<double>& p, int i) {
        auto sig1 = s1(p, i);
        auto sig2 = s2(p, i);
        return (sig1 == sig2) ? sig1 : 0;
    };
}

// Invert a strategy's signals
Strategy invert(Strategy s) {
    return [s](const std::vector<double>& p, int i) {
        return -s(p, i);
    };
}
```

Higher-order functions take and return functions. Lambdas are the tool of choice for building these compositions.

---

## Step 5 — Running the Backtest

```cpp
double backtest(const std::vector<double>& prices,
                const Strategy& strategy,
                double position_size) {
    double pnl = 0.0;
    int position = 0;  // 0 = flat, 1 = long, -1 = short

    for (int i = 1; i < (int)prices.size(); ++i) {
        int signal = strategy(prices, i);
        if (signal == 1 && position == 0) position = 1;
        if (signal == -1 && position == 1) {
            pnl += (prices[i] - prices[i-1]) * position_size;
            position = 0;
        }
    }
    return pnl;
}
```
