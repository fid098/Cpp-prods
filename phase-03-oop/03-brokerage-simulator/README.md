# 03 — Brokerage Simulator

> Phase 3 · OOP | C++23 | Simulate broker accounts, orders, fills, and fees via polymorphism

## What You'll Build

A brokerage class hierarchy: a base `Account` with derived `CashAccount` and `MarginAccount`, each with different fee schedules. Orders are submitted, filled, and fees are deducted automatically.

```
=== Brokerage Simulator ===
Cash account  balance=$10000.00
  BUY  AAPL 100 @ 150.00  fee=$1.50  filled ✓  balance=$8498.50
  SELL AAPL 50  @ 160.00  fee=$1.60  filled ✓  balance=$16496.90

Margin account balance=$10000.00 (2x leverage)
  BUY  TSLA 20  @ 700.00  fee=$0.70  filled ✓  balance=$-4000.70
```

## Learning Objectives

- Design a class hierarchy with `virtual` methods
- Use `override` on derived class methods
- Understand `protected` vs `private` vs `public` access
- Implement polymorphic dispatch via base class pointer/reference
- Use `dynamic_cast` for safe downcasting

## Requirements

1. `Account` base: `balance()`, virtual `fee(double notional)`, virtual `submit_order(...)`, `fill_order(...)`
2. `CashAccount`: fee = 0.01% of notional (min $1.00)
3. `MarginAccount`: fee = 0.005% of notional; maintains leverage ratio; rejects orders that breach limit
4. `Order` struct: symbol, side, qty, price, status enum (PENDING/FILLED/REJECTED)
5. Polymorphic `process_orders(Account&, vector<Order>&)` fills all orders

## Key Resources

| Concept | Link |
|---------|------|
| Inheritance | https://en.cppreference.com/w/cpp/language/derived_class |
| Virtual functions | https://en.cppreference.com/w/cpp/language/virtual |
| protected | https://en.cppreference.com/w/cpp/language/access |
| dynamic_cast | https://en.cppreference.com/w/cpp/language/dynamic_cast |
