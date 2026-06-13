# 04 — Factor Ranking Engine

> Phase 4 · STL | C++23 | Rank securities by factor scores and rebalance a model portfolio

## What You'll Build

A factor-based ranking engine: given a universe of stocks with momentum, value, and quality scores, compute a composite score, rank all stocks, and output a target portfolio allocation.

```
=== Factor Ranking Engine ===
Ranked universe (composite score):
  1. NVDA   mom=0.90  val=0.60  qual=0.85  composite=0.783
  2. AAPL   mom=0.75  val=0.70  qual=0.80  composite=0.750
  ...

Top 5 equal-weight portfolio:
  NVDA  20%   AAPL  20%  ...
```

## Learning Objectives

- Use `std::sort` with a custom comparator (by composite score)
- Use `std::partial_sort` to get the top N without sorting everything
- Use `std::transform` to compute composite scores from factor scores
- Use `std::inner_product` to compute dot products

## Key Resources

| Concept | Link |
|---------|------|
| std::sort | https://en.cppreference.com/w/cpp/algorithm/sort |
| std::partial_sort | https://en.cppreference.com/w/cpp/algorithm/partial_sort |
| std::transform | https://en.cppreference.com/w/cpp/algorithm/transform |
| std::inner_product | https://en.cppreference.com/w/cpp/algorithm/inner_product |

## Requirements

1. `Stock` struct: symbol, momentum, value, quality scores (all 0–1)
2. `composite_score(stock, weights)` — weighted sum of factor scores
3. `rank_universe(stocks, weights)` — returns stocks sorted by composite score desc
4. `top_n(stocks, n)` — first N after ranking
5. `equal_weight_allocation(n)` — returns 1.0/n for each top holding
