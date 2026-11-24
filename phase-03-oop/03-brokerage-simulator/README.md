# 03 — Brokerage Simulator

> Phase 3 · OOP | C++23 | Simulate broker accounts, orders, fills, and fees via polymorphism

## What You'll Build

A brokerage class hierarchy: a base `Account` with derived `CashAccount` and `MarginAccount`, each with different fee schedules. Orders are submitted, filled, and fees are deducted automatically.

```
=== Brokerage Simulator ===

Cash account  opening balance=$10000.00
  BUY  AAPL   50 @  150.00  fee=$ 1.00  FILLED    balance=$2499.00
  BUY  TSLA   10 @  700.00  fee=$ 0.00  REJECTED  balance=$2499.00
  SELL AAPL   50 @  160.00  fee=$ 1.00  FILLED    balance=$10498.00

Margin account  opening balance=$10000.00
  BUY  TSLA   20 @  700.00  fee=$ 0.70  FILLED    balance=$-4000.70
  BUY  NVDA   10 @  800.00  fee=$ 0.00  REJECTED  balance=$-4000.70
  SELL TSLA   20 @  720.00  fee=$ 0.72  FILLED    balance=$10398.58

--- Fee on a $100,000 trade (virtual dispatch) ---
  Cash   fee=$10.00
  Margin fee=$5.00

--- Margin details (dynamic_cast) ---
  Cash: not a margin account (cast returned nullptr)
  Margin: leverage=2.00x  credit_limit=$10000.00  buying_power=$20398.58
```

## Learning Objectives

- Design a class hierarchy with `virtual` methods
- Use `override` on derived class methods
- Understand `protected` vs `private` vs `public` access
- Implement polymorphic dispatch via base class pointer/reference
- Use `dynamic_cast` for safe downcasting

## Requirements

1. `Account` base: `balance()`, virtual `fee(double notional)`, virtual `submit_order(...)`, protected `fill_order(...)`
2. `CashAccount`: fee = 0.01% of notional (min $1.00)
3. `MarginAccount`: fee = 0.005% of notional (min $0.50); may borrow up to `(leverage − 1) × deposit`; rejects orders that breach that limit
4. `Order` struct: symbol, side, qty, price, status enum (PENDING/FILLED/REJECTED)
5. Polymorphic `process_orders(Account&, vector<Order>&)` fills all orders

## Key Resources

| Concept | Link |
|---------|------|
| Inheritance | https://en.cppreference.com/w/cpp/language/derived_class |
| Virtual functions | https://en.cppreference.com/w/cpp/language/virtual |
| protected | https://en.cppreference.com/w/cpp/language/access |
| dynamic_cast | https://en.cppreference.com/w/cpp/language/dynamic_cast |
