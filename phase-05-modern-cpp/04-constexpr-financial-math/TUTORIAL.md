# Tutorial — Constexpr Financial Math

## Step 1 — constexpr Functions

A `constexpr` function can be called with compile-time-known arguments and its result embedded in the binary as a constant:

```cpp
constexpr double bps_to_pct(double bps) {
    return bps / 10000.0;
}

constexpr double spread_pct = bps_to_pct(50.0);  // 0.005, computed at compile time
```

The same function still works at runtime:
```cpp
double user_bps = get_from_input();
double runtime_pct = bps_to_pct(user_bps);  // evaluated at runtime
```

> **Docs:** https://en.cppreference.com/w/cpp/language/constexpr

---

## Step 2 — consteval: Compile-Time Only

`consteval` (C++20) forces evaluation at compile time — calling it with a runtime value is a compile error:

```cpp
consteval int days_in_year(bool leap) {
    return leap ? 366 : 365;
}

constexpr int d1 = days_in_year(false);  // OK
// int d2 = days_in_year(runtime_bool);  // ERROR: consteval requires compile-time arg
```

Use `consteval` for things that must be constant: look-up table generators, configuration validation.

> **Docs:** https://en.cppreference.com/w/cpp/language/consteval

---

## Step 3 — static_assert

```cpp
static_assert(bps_to_pct(10000.0) == 1.0, "10000 bps must equal 100%");
static_assert(days_in_year(false) == 365);
```

`static_assert` fires at compile time if the condition is false — catches configuration errors before your code runs.

> **Docs:** https://en.cppreference.com/w/cpp/language/static_assert

---

## Step 4 — Compile-Time Discount Factor

`constexpr` functions cannot call non-`constexpr` functions (like `std::exp` in older standards), but in C++26 `std::exp` will be `constexpr`. For now, use a Taylor series:

```cpp
// exp(x) ≈ 1 + x + x²/2! + x³/3! + ... (first 15 terms, good to ~12 decimals)
constexpr double constexpr_exp(double x, int terms = 15) {
    double result = 1.0, term = 1.0;
    for (int n = 1; n < terms; ++n) {
        term *= x / n;
        result += term;
    }
    return result;
}

constexpr double discount_factor(double rate, double T) {
    return constexpr_exp(-rate * T);
}

constexpr double df_10yr = discount_factor(0.05, 10.0);
static_assert(df_10yr > 0.0 && df_10yr < 1.0);
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Calling non-constexpr function in constexpr | Provide a constexpr alternative (Taylor series, lookup table) |
| Expecting `const` to enable compile-time evaluation | `const` at runtime is not `constexpr` — use `constexpr` explicitly |
| Making `consteval` functions too general | Reserve `consteval` for truly mandatory compile-time computations |
