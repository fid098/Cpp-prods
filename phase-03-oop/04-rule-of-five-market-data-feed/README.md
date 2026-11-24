# 04 — Rule of Five: Market Data Feed

> Phase 3 · OOP | C++23 | Safe resource ownership for a market data feed

## What You'll Build

A `MarketDataFeed` class that owns a heap-allocated buffer of `Tick` data. You implement all five special member functions — destructor, copy constructor, copy assignment, move constructor, move assignment — and verify that each does the right thing.

```
=== Rule of Five: Market Data Feed ===

feed1  capacity=1000  ticks loaded: 1000
feed2 (copy of feed1)  capacity=1000  independent buffer: YES
feed3 (move from feed2)  capacity=1000  feed2 is now empty: YES  buffer stolen (same address): YES
feed4 = feed1  capacity 5 -> 1000
feed4 = std::move(feed3)  feed4[0].price=-1  feed3 empty: YES
After self-assignment: feed1 capacity=1000  size=1000
Reused moved-from feed2: capacity=3 push ok: YES

  [after demos] allocations=5 frees=2 copies=2 moves=3

5 x emplace_back into a vector (it reallocates as it grows):
  copies=0 moves=7  -> elements were MOVED, never copied, because the move ctor is noexcept

  [after scope] allocations=10 frees=10 copies=2 moves=10
Leak check: live buffers = 0  (no leaks)
```

## Learning Objectives

- Implement a destructor that releases heap resources
- Understand shallow vs deep copy and why you need a custom copy constructor
- Implement copy constructor (deep copy)
- Implement copy assignment operator with self-assignment guard
- Implement move constructor (steal resources; leave source in valid empty state)
- Implement move assignment operator
- Make copy assignment exception-safe (allocate before you free)
- See why `noexcept` moves matter to `std::vector`

## The Rule of Five

If you define **any** of: destructor, copy constructor, copy assignment, move constructor, move assignment — you almost certainly need to define **all five**.

| Special member | Purpose |
|----------------|---------|
| Destructor | Release owned resource |
| Copy constructor | Deep copy |
| Copy assignment | Deep copy + self-assign guard; allocate new buffer *before* freeing old |
| Move constructor | Steal pointer; null out source |
| Move assignment | Steal + self-assign guard; null out source |

## Key Resources

| Concept | Link |
|---------|------|
| Rule of three/five/zero | https://en.cppreference.com/w/cpp/language/rule_of_three |
| Move semantics | https://en.cppreference.com/w/cpp/language/move_constructor |
| std::exchange | https://en.cppreference.com/w/cpp/utility/exchange |
| noexcept | https://en.cppreference.com/w/cpp/language/noexcept |
