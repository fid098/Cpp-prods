# Tutorial — Portfolio Analytics Dashboard

## Step 1 — The Holding Struct

```cpp
struct Holding {
    std::string symbol;
    std::string sector;
    int         qty;
    double      price;
    double      value() const { return qty * price; }
};
```

---

## Step 2 — Total Value with std::accumulate

```cpp
#include <numeric>

double total_value(const std::vector<Holding>& holdings) {
    return std::accumulate(holdings.begin(), holdings.end(), 0.0,
        [](double sum, const Holding& h) { return sum + h.value(); });
}
```

`std::accumulate(first, last, init, op)` applies `op(sum, element)` left to right.

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/accumulate

---

## Step 3 — Weights with std::transform

```cpp
std::vector<double> weights(const std::vector<Holding>& h) {
    double total = total_value(h);
    std::vector<double> w(h.size());
    std::transform(h.begin(), h.end(), w.begin(),
        [total](const Holding& x) { return x.value() / total; });
    return w;
}
```

`std::transform` applies a function to each element and writes the result to an output range.

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/transform

---

## Step 4 — Sort by Value Descending

```cpp
void sort_by_value(std::vector<Holding>& h) {
    std::sort(h.begin(), h.end(),
        [](const Holding& a, const Holding& b) {
            return a.value() > b.value();  // descending
        });
}
```

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/sort

---

## Step 5 — Herfindahl Index (Concentration)

```
HHI = Σ weight_i²
```

HHI = 1.0 means one holding owns everything (maximally concentrated). HHI < 0.25 is considered diversified.

```cpp
double herfindahl(const std::vector<Holding>& h) {
    double total = total_value(h);
    return std::accumulate(h.begin(), h.end(), 0.0,
        [total](double sum, const Holding& x) {
            double w = x.value() / total;
            return sum + w * w;
        });
}
```

---

## Step 6 — Sector Exposure with std::map

```cpp
std::map<std::string, double> sector_exposure(const std::vector<Holding>& h) {
    double total = total_value(h);
    std::map<std::string, double> exposure;
    for (const auto& holding : h)
        exposure[holding.sector] += holding.value() / total;
    return exposure;
}
```
