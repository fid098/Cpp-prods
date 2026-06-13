# 04 — Constexpr Financial Math

> Phase 5 · Modern C++ | C++23 | Compile-time constants, rates, and unit conversions

## What You'll Build

A header-only library of financial math constants and conversion utilities computed **at compile time** using `constexpr` and `consteval`. Zero runtime cost for any conversion that uses compile-time-known inputs.

```cpp
// All of these are computed at compile time — zero runtime cost:
constexpr double ANNUAL_RATE = 0.05;
constexpr double DAILY_RATE  = annual_to_daily(ANNUAL_RATE);   // 0.000198...
constexpr double DISCOUNT    = discount_factor(ANNUAL_RATE, 10); // 0.6065...
constexpr double BPS_TO_PCT  = bps_to_pct(150);                 // 0.015

static_assert(DAILY_RATE > 0.0, "must be positive");
```

## Learning Objectives

- Mark functions `constexpr` to enable compile-time evaluation
- Use `consteval` for functions that MUST be evaluated at compile time
- Use `static_assert` to verify compile-time conditions
- Understand `constexpr` variables vs `const` vs runtime `const`
- Know the limits: `constexpr` functions can still be called at runtime

## Key Resources

| Concept | Link |
|---------|------|
| constexpr | https://en.cppreference.com/w/cpp/language/constexpr |
| consteval | https://en.cppreference.com/w/cpp/language/consteval |
| static_assert | https://en.cppreference.com/w/cpp/language/static_assert |
| if constexpr | https://en.cppreference.com/w/cpp/language/if#Constexpr_if |
