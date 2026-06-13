# 05 — Concepts for Data Feeds

> Phase 5 · Modern C++ | C++23 | Constrain feed and strategy interfaces using C++20 concepts

## What You'll Build

A type-safe data feed and strategy framework. Concepts enforce that any type you plug in as a "feed" or "strategy" provides the required interface — bad types produce clear compile errors instead of cryptic template errors.

```cpp
// If DataFeed is satisfied, this compiles:
static_assert(DataFeed<TickFeed>);

// If Strategy is not satisfied (missing price_signal method), you get:
// error: constraints not satisfied for type 'BadStrategy'
```

## Learning Objectives

- Define a `concept` using `requires` expressions
- Constrain a function template with `requires` or the shorthand syntax
- Use `static_assert` to verify a type satisfies a concept at compile time
- Understand the difference between concepts and virtual interfaces
- Use `std::same_as`, `std::convertible_to` from `<concepts>`

## Key Resources

| Concept (language) | Link |
|--------------------|------|
| Concepts | https://en.cppreference.com/w/cpp/language/constraints |
| requires expression | https://en.cppreference.com/w/cpp/language/requires |
| Standard concepts | https://en.cppreference.com/w/cpp/concepts |
| std::same_as | https://en.cppreference.com/w/cpp/concepts/same_as |
| Abbreviated function templates | https://en.cppreference.com/w/cpp/language/function_template |
