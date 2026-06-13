# Tutorial — Limit Order Book

## Step 1 — Why std::map?

Bids must always be accessible in **price descending** order (best bid first); asks in **price ascending** order (best ask first).

`std::map<double, Level>` keeps entries sorted by key automatically:

```cpp
std::map<double, int> bids;  // price → total qty
bids[99.50] = 200;
bids[99.25] = 500;

// Highest price: rbegin() (reverse iterator)
auto best = bids.rbegin();   // points to {99.50, 200}
```

> **Docs:** https://en.cppreference.com/w/cpp/container/map

---

## Step 2 — Data Structures

```cpp
struct Order {
    int    id;
    double price;
    int    qty;
    bool   is_bid;
};

struct PriceLevel {
    int total_qty{0};
    std::queue<int> order_ids;  // FIFO within a price level
};
```

The book keeps:
- `std::map<double, PriceLevel> bids_` — sorted ascending, use `rbegin()` for best
- `std::map<double, PriceLevel> asks_` — sorted ascending, use `begin()` for best
- `std::unordered_map<int, Order> orders_` — fast lookup by order ID

---

## Step 3 — Adding an Order

```cpp
void add_order(int id, bool is_bid, double price, int qty) {
    Order o{id, price, qty, is_bid};
    orders_[id] = o;

    auto& book  = is_bid ? bids_ : asks_;
    auto& level = book[price];  // creates level if not present
    level.total_qty += qty;
    level.order_ids.push(id);
}
```

---

## Step 4 — Cancelling an Order

```cpp
void cancel_order(int id) {
    auto it = orders_.find(id);
    if (it == orders_.end()) return;  // not found

    const Order& o = it->second;
    auto& book  = o.is_bid ? bids_ : asks_;
    auto  level_it = book.find(o.price);
    if (level_it != book.end()) {
        level_it->second.total_qty -= o.qty;
        if (level_it->second.total_qty <= 0)
            book.erase(level_it);   // remove empty level
    }
    orders_.erase(it);
}
```

---

## Step 5 — Best Bid / Ask

```cpp
double best_bid() const {
    if (bids_.empty()) return 0.0;
    return bids_.rbegin()->first;   // highest price key
}

double best_ask() const {
    if (asks_.empty()) return 0.0;
    return asks_.begin()->first;    // lowest price key
}
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Using `begin()` for best bid | Bids: use `rbegin()` for the highest price |
| Not removing empty price levels | Check `total_qty <= 0` after cancel and erase |
| Sorting asks in reverse | `std::map` sorts ascending by default — asks are correct as-is |
