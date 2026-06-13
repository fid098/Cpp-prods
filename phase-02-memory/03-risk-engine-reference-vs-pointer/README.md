# 03 — Risk Engine: Reference vs Pointer

> Phase 2 · Memory | C++23 | Update risk metrics efficiently using references and pointers

## What You'll Build

A risk metrics update engine that demonstrates the key difference between passing by value, by pointer, and by reference. You will implement functions that efficiently update a `RiskMetrics` struct in place, and see — via benchmark numbers — the cost of unnecessary copies.

```
=== Risk Engine: Reference vs Pointer ===

Initial metrics: var=0.00  beta=0.00  sharpe=0.00

update_by_value    (copy): var=0.05  beta=1.20  sharpe=1.80  [caller unchanged!]
update_by_pointer  (ptr):  var=0.05  beta=1.20  sharpe=1.80  [caller updated ✓]
update_by_reference(ref):  var=0.03  beta=0.95  sharpe=2.10  [caller updated ✓]

Demonstrating const ref (read-only, no copy):
  format_metrics(&m) = "VAR=0.03 BETA=0.95 SHARPE=2.10"
```

## Learning Objectives

- Understand pass-by-value: a copy is made; the original is unchanged
- Understand pass-by-pointer: caller passes `&x`; function uses `*` or `->`
- Understand pass-by-reference: an alias; no copy, no pointer syntax for the caller
- Know when to use `const T&` (read-only, no copy) vs `T&` (mutable) vs `T*` (nullable)
- Measure the cost of copying large structs

## Project Structure

```
03-risk-engine-reference-vs-pointer/
├── include/
│   └── risk_metrics.hpp
├── src/
│   └── main.cpp
├── tests/
│   └── test_risk_metrics.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. Define `RiskMetrics`: `double var`, `double beta`, `double sharpe`, `double alpha`, `double max_dd`
2. `update_by_value(RiskMetrics m, ...)` — takes a copy; changes do NOT affect the caller
3. `update_by_pointer(RiskMetrics* m, ...)` — takes address; changes affect the caller
4. `update_by_reference(RiskMetrics& m, ...)` — takes reference; changes affect the caller
5. `format_metrics(const RiskMetrics& m)` — read-only, returns a formatted string, no copy
6. Demonstrate in main() that value-passing leaves the original unchanged

## Key Resources

| Concept | Link |
|---------|------|
| References | https://en.cppreference.com/w/cpp/language/reference |
| Pointers | https://en.cppreference.com/w/cpp/language/pointer |
| Pass by value/ref/pointer | https://en.cppreference.com/w/cpp/language/function#Pass_by_value |
| const correctness | https://isocpp.org/wiki/faq/const-correctness |
