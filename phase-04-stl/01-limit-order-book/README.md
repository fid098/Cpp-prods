# 01 — Limit Order Book

> Phase 4 · STL | C++23 | Price-time priority order book with `std::map`

## What You'll Build

A **limit order book (LOB)** — the core data structure of every exchange. Bids and asks are stored in sorted price levels; the best bid is the highest price and the best ask is the lowest.

```
=== Limit Order Book ===
ADD BID  AAPL  99.50  qty=200  id=1
ADD BID  AAPL  99.25  qty=500  id=2
ADD ASK  AAPL 100.00  qty=300  id=3
ADD ASK  AAPL 100.25  qty=150  id=4

Order Book — AAPL
  ASKS:
    100.25   150
    100.00   300
  --------
  BIDS:
     99.50   200
     99.25   500

Best bid: 99.50  Best ask: 100.00  Spread: 0.50
```

## Learning Objectives

- Use `std::map<double, Queue>` for sorted price levels
- Use `std::unordered_map<int, OrderRef>` for O(1) order lookup by ID
- Understand map iterators: `begin()`, `end()`, `rbegin()`, `rend()`
- Implement add, cancel, and match operations
- Understand the difference between `map` (sorted) and `unordered_map` (O(1) lookup)

## Requirements

1. `add_order(id, side, price, qty)` — inserts at the right price level
2. `cancel_order(id)` — removes by ID in O(1) using the lookup map
3. `best_bid()` / `best_ask()` — return the best prices
4. `spread()` — `best_ask - best_bid`
5. `match()` — if best bid >= best ask, execute a trade and remove filled qty
6. `print_book()` — print asks (low→high) then bids (high→low)

## Key Resources

| Concept | Link |
|---------|------|
| std::map | https://en.cppreference.com/w/cpp/container/map |
| std::unordered_map | https://en.cppreference.com/w/cpp/container/unordered_map |
| Map iterators (rbegin/rend) | https://en.cppreference.com/w/cpp/container/map/rbegin |
| std::queue | https://en.cppreference.com/w/cpp/container/queue |
