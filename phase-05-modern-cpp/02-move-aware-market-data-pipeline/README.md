# 02 — Move-Aware Market Data Pipeline

> Phase 5 · Modern C++ | C++23 | Move large tick batches through a processing pipeline cheaply

## What You'll Build

A pipeline of processing stages that passes `TickBatch` objects — each holding thousands of ticks — between stages using **move semantics** to avoid expensive copies.

```
=== Market Data Pipeline ===
Stage 1: Ingest    200000 ticks  (moved in, 0 copies)
Stage 2: Validate  198500 ticks  (moved, 1500 filtered)
Stage 3: Enrich    198500 ticks  (moved, VWAP computed)
Stage 4: Publish   198500 ticks  (moved to sink)
Total copies of tick data: 0
```

## Learning Objectives

- Write a move constructor and move assignment
- Use `std::move` to transfer ownership without copying
- Understand lvalue vs rvalue references (`T&` vs `T&&`)
- Use `std::move` when passing large objects to functions
- Understand when the compiler uses the move constructor automatically (NRVO)

## Key Resources

| Concept | Link |
|---------|------|
| Move constructors | https://en.cppreference.com/w/cpp/language/move_constructor |
| Rvalue references | https://en.cppreference.com/w/cpp/language/rvalue_reference |
| std::move | https://en.cppreference.com/w/cpp/utility/move |
| Value categories | https://en.cppreference.com/w/cpp/language/value_category |
| NRVO | https://en.cppreference.com/w/cpp/language/copy_elision |
