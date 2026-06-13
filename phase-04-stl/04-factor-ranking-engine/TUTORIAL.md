# Tutorial — Factor Ranking Engine

## Step 1 — Composite Score

A composite score weights three factors:

```cpp
struct FactorWeights { double momentum, value, quality; };

double composite_score(const Stock& s, const FactorWeights& w) {
    return s.momentum * w.momentum
         + s.value    * w.value
         + s.quality  * w.quality;
}
```

With `std::inner_product`:
```cpp
#include <numeric>
std::array<double,3> scores  = {s.momentum, s.value, s.quality};
std::array<double,3> weights = {w.momentum, w.value, w.quality};
return std::inner_product(scores.begin(), scores.end(), weights.begin(), 0.0);
```

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/inner_product

---

## Step 2 — std::sort with Custom Comparator

```cpp
std::vector<Stock> rank_universe(std::vector<Stock> stocks,
                                  const FactorWeights& w) {
    std::sort(stocks.begin(), stocks.end(),
        [&w](const Stock& a, const Stock& b) {
            return composite_score(a, w) > composite_score(b, w);  // desc
        });
    return stocks;
}
```

The lambda captures `w` by reference (`[&w]`).

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/sort

---

## Step 3 — std::partial_sort for Top N

```cpp
void partial_rank(std::vector<Stock>& stocks, int n,
                   const FactorWeights& w) {
    if (n >= static_cast<int>(stocks.size())) n = stocks.size();
    std::partial_sort(stocks.begin(), stocks.begin() + n, stocks.end(),
        [&w](const Stock& a, const Stock& b) {
            return composite_score(a, w) > composite_score(b, w);
        });
    stocks.resize(n);
}
```

`std::partial_sort` only sorts the first N elements — more efficient than sorting everything.

> **Docs:** https://en.cppreference.com/w/cpp/algorithm/partial_sort
