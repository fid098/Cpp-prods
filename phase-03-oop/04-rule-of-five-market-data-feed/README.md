# 04 — Rule of Five: Market Data Feed

> Phase 3 · OOP | C++23 | Safe resource ownership for a market data feed

## What You'll Build

A `MarketDataFeed` class that owns a heap-allocated buffer of `Tick` data. You implement all five special member functions — destructor, copy constructor, copy assignment, move constructor, move assignment — and verify that each does the right thing.

```
=== Rule of Five: Market Data Feed ===
feed1  capacity=1000  ticks loaded: 1000
feed2 (copy of feed1)  capacity=1000  independent buffer: YES
feed3 (move from feed2)  capacity=1000  feed2 is now empty: YES
After self-assignment: feed1 capacity=1000
```

## Learning Objectives

- Implement a destructor that releases heap resources
- Understand shallow vs deep copy and why you need a custom copy constructor
- Implement copy constructor (deep copy)
- Implement copy assignment operator with self-assignment guard
- Implement move constructor (steal resources; leave source in valid empty state)
- Implement move assignment operator

## The Rule of Five

If you define **any** of: destructor, copy constructor, copy assignment, move constructor, move assignment — you almost certainly need to define **all five**.

| Special member | Purpose |
|----------------|---------|
| Destructor | Release owned resource |
| Copy constructor | Deep copy |
| Copy assignment | Deep copy + self-assign guard |
| Move constructor | Steal pointer; null out source |
| Move assignment | Steal + self-assign guard; null out source |

## Key Resources

| Concept | Link |
|---------|------|
| Rule of three/five/zero | https://en.cppreference.com/w/cpp/language/rule_of_three |
| Move semantics | https://en.cppreference.com/w/cpp/language/move_constructor |
| std::exchange | https://en.cppreference.com/w/cpp/utility/exchange |
| noexcept | https://en.cppreference.com/w/cpp/language/noexcept |
