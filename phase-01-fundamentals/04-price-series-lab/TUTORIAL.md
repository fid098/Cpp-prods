# Tutorial — Price Series Lab

## Step 1 — Daily Returns

A **daily return** tells you how much a price changed from one day to the next, as a percentage:

```
return(t) = (price[t] - price[t-1]) / price[t-1] * 100
```

In code:
```cpp
inline double daily_return(double prev, double curr) {
    return (curr - prev) / prev * 100.0;
}
```

To compute all returns for a series, loop from index `1` to `n-1`:

```cpp
for (size_t i = 1; i < n; ++i) {
    double r = daily_return(prices[i - 1], prices[i]);
}
```

> **Docs:** https://en.cppreference.com/w/cpp/language/for

---

## Step 2 — High and Low

Iterate through the array keeping a running maximum and minimum. Start your max at the smallest possible `double` and your min at the largest:

```cpp
#include <limits>

inline double series_high(const double* prices, size_t n) {
    double high = std::numeric_limits<double>::lowest(); // most negative double
    for (size_t i = 0; i < n; ++i) {
        if (prices[i] > high) high = prices[i];
    }
    return high;
}
```

> **Docs:** https://en.cppreference.com/w/cpp/types/numeric_limits

Alternatively use `std::numeric_limits<double>::max()` as the starting min.

---

## Step 3 — Maximum Drawdown

**Max drawdown** is the largest percentage drop from any peak to any subsequent trough.

```
peak     = running maximum up to today
drawdown = (today's price - peak) / peak * 100
```

Because price < peak, the drawdown value is negative or zero.

```cpp
inline double max_drawdown(const double* prices, size_t n) {
    double peak        = prices[0];
    double max_dd      = 0.0;

    for (size_t i = 1; i < n; ++i) {
        if (prices[i] > peak) peak = prices[i];      // new peak
        double dd = (prices[i] - peak) / peak * 100.0;
        if (dd < max_dd) max_dd = dd;                // more negative = worse
    }
    return max_dd;
}
```

---

## Step 4 — Average Return

Sum all daily returns and divide by the count:

```cpp
inline double average_return(const double* prices, size_t n) {
    if (n < 2) return 0.0;

    double sum = 0.0;
    for (size_t i = 1; i < n; ++i) {
        sum += daily_return(prices[i - 1], prices[i]);
    }
    return sum / static_cast<double>(n - 1);
}
```

---

## Step 5 — Using std::array in main.cpp

`std::array` is the modern, fixed-size array with bounds-checking in debug builds:

```cpp
#include <array>
#include <iostream>
#include <iomanip>
#include "price_series.hpp"

int main() {
    std::array<double, 7> prices = {100.0, 102.5, 101.0, 105.0, 98.0, 107.0, 103.5};

    // Pass the underlying array pointer and size to your functions:
    const double* p = prices.data();
    size_t        n = prices.size();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "High:          " << series_high(p, n)      << "\n";
    std::cout << "Low:           " << series_low(p, n)       << "\n";
    std::cout << "Max Drawdown:  " << max_drawdown(p, n)     << "%\n";
    std::cout << "Avg Return:    " << average_return(p, n)   << "%\n";
    return 0;
}
```

> **Docs:** https://en.cppreference.com/w/cpp/container/array

---

## Step 6 — Pointer-to-Array Decay

When you pass an array to a function expecting `const double*`, the array **decays** to a pointer to its first element. This is fundamental C++ behaviour. The function does not know the length — that is why you always pass `n` alongside:

```cpp
void print_prices(const double* prices, size_t n) {
    for (size_t i = 0; i < n; ++i)
        std::cout << prices[i] << " ";
}
```

> **Docs:** https://en.cppreference.com/w/cpp/language/array#Array-to-pointer_decay

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Starting `series_high` at 0.0 | Use `std::numeric_limits<double>::lowest()` so negative prices still work |
| Off-by-one in return loop | Returns are computed for indices `1` to `n-1` — that is `n-1` returns total |
| Not updating peak before computing drawdown | Update peak first, then compute `dd` |
| Integer division when computing return | Ensure at least one operand is `double` |
