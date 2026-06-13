# Phase 5 — Modern C++

**C++ concepts covered:** `unique_ptr`, `shared_ptr`, `weak_ptr`, rvalue references, `std::move`, move constructors, lambdas, captures, `auto`, `constexpr`, `consteval`, concepts (C++20), template constraints

Phase 5 is where idiomatic modern C++ starts. You will eliminate raw pointers, move data efficiently, compose behaviour with lambdas, and make the compiler enforce your type constraints.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [smart-pointer-pricing-graph](01-smart-pointer-pricing-graph/) | Dependency graph for pricers | `unique_ptr`, `shared_ptr`, `weak_ptr` |
| 02 | [move-aware-market-data-pipeline](02-move-aware-market-data-pipeline/) | Move tick batches cheaply | rvalue refs, `std::move`, ownership |
| 03 | [lambda-strategy-backtester](03-lambda-strategy-backtester/) | Compose trading rules with lambdas | lambdas, captures, `auto` |
| 04 | [constexpr-financial-math](04-constexpr-financial-math/) | Compile-time rates and conversions | `constexpr`, `consteval` |
| 05 | [concepts-for-data-feeds](05-concepts-for-data-feeds/) | Constrained feed/strategy interfaces | concepts, C++20/23 template interfaces |

## Phase Resources

- https://en.cppreference.com/w/cpp/memory/unique_ptr
- https://en.cppreference.com/w/cpp/memory/shared_ptr
- https://en.cppreference.com/w/cpp/language/rvalue_reference
- https://en.cppreference.com/w/cpp/language/lambda
- https://en.cppreference.com/w/cpp/language/constexpr
- https://en.cppreference.com/w/cpp/language/constraints
