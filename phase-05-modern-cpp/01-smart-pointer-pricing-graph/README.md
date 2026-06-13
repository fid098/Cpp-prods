# 01 — Smart Pointer Pricing Graph

> Phase 5 · Modern C++ | C++23 | Dependency graph for pricing components using smart pointers

## What You'll Build

A directed acyclic graph (DAG) of pricing nodes. Each node computes a price based on its dependencies. Ownership is managed entirely through smart pointers — no `new` or `delete`.

```
=== Pricing Graph ===
SpotNode     AAPL   price=175.00  (leaf — no dependencies)
VolNode      AAPL   vol=0.20      (leaf)
ForwardNode  AAPL   price=177.19  (depends on SpotNode + risk-free rate)
OptionNode   AAPL   price=4.23    (depends on ForwardNode + VolNode)
```

## Learning Objectives

- Use `std::unique_ptr` for sole ownership (no sharing)
- Use `std::shared_ptr` for shared ownership (reference counted)
- Use `std::weak_ptr` to break ownership cycles
- Understand `std::make_unique` and `std::make_shared`
- Understand when each smart pointer is appropriate

## Ownership Rules

| Smart pointer | Rule |
|---------------|------|
| `unique_ptr<T>` | Exactly one owner. Move-only. Use for tree structures. |
| `shared_ptr<T>` | Multiple owners. Ref-counted. Use when ownership is shared. |
| `weak_ptr<T>` | Non-owning observer. Must be locked before use. Use to break cycles. |

## Key Resources

| Concept | Link |
|---------|------|
| unique_ptr | https://en.cppreference.com/w/cpp/memory/unique_ptr |
| shared_ptr | https://en.cppreference.com/w/cpp/memory/shared_ptr |
| weak_ptr | https://en.cppreference.com/w/cpp/memory/weak_ptr |
| make_unique | https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique |
| make_shared | https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared |
