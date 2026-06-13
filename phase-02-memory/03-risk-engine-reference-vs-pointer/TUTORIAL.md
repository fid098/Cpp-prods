# Tutorial — Risk Engine: Reference vs Pointer

## Step 1 — The Three Passing Modes

### Pass by Value (copy)

```cpp
void update_by_value(RiskMetrics m, double new_var) {
    m.var = new_var;  // modifies the LOCAL copy — caller never sees this
}

RiskMetrics metrics{0.0, 1.0, 1.5};
update_by_value(metrics, 0.05);
// metrics.var is STILL 0.0 here
```

> **Use when:** the function needs its own copy of the data (rare for large structs).

---

### Pass by Pointer

```cpp
void update_by_pointer(RiskMetrics* m, double new_var) {
    m->var = new_var;  // m->var is shorthand for (*m).var
}

RiskMetrics metrics{0.0, 1.0, 1.5};
update_by_pointer(&metrics, 0.05);  // pass the address
// metrics.var IS now 0.05
```

The caller must write `&metrics` and the function must check for `nullptr`. Use pointers when the argument might legitimately be absent (nullable).

> **Docs:** https://en.cppreference.com/w/cpp/language/pointer

---

### Pass by Reference

```cpp
void update_by_reference(RiskMetrics& m, double new_var) {
    m.var = new_var;  // m is an alias for the caller's variable
}

RiskMetrics metrics{0.0, 1.0, 1.5};
update_by_reference(metrics, 0.05);  // no &, no *
// metrics.var IS now 0.05
```

A reference is like an alias — `m` IS `metrics`, just with a different name inside the function. References cannot be null.

> **Docs:** https://en.cppreference.com/w/cpp/language/reference

---

### const Reference (read-only, no copy)

```cpp
std::string format_metrics(const RiskMetrics& m) {
    // m is read-only — no copy is made
    return "VAR=" + std::to_string(m.var) + " BETA=" + std::to_string(m.beta);
}
```

**Rule of thumb:**
- Input parameters of class/struct type → `const T&`
- Output parameters → `T&` (or return by value)
- Optional parameters → `T*` (can be `nullptr`)
- Small built-in types (`int`, `double`) → pass by value

---

## Step 2 — Implement the Functions

In `include/risk_metrics.hpp`:

```cpp
struct RiskMetrics {
    double var{0.0};
    double beta{0.0};
    double sharpe{0.0};
    double alpha{0.0};
    double max_dd{0.0};
};

// Demonstrates pass-by-value (caller NOT modified)
inline void update_by_value(RiskMetrics m,
                             double var, double beta, double sharpe) {
    m.var = var; m.beta = beta; m.sharpe = sharpe;
    // local m is discarded on return
}

// Demonstrates pass-by-pointer (caller IS modified)
inline void update_by_pointer(RiskMetrics* m,
                               double var, double beta, double sharpe) {
    // TODO: guard against nullptr, then set m->var, m->beta, m->sharpe
}

// Demonstrates pass-by-reference (caller IS modified)
inline void update_by_reference(RiskMetrics& m,
                                 double var, double beta, double sharpe) {
    // TODO: set m.var, m.beta, m.sharpe
}

// Read-only — returns a formatted string, no copy of m
inline std::string format_metrics(const RiskMetrics& m) {
    // TODO: use std::to_string or a stringstream
    return {};
}
```

---

## Step 3 — Main Demonstration

```cpp
int main() {
    RiskMetrics m{};

    std::cout << "Before: " << format_metrics(m) << "\n";

    update_by_value(m, 0.05, 1.2, 1.8);
    std::cout << "After update_by_value: " << format_metrics(m) << "\n";
    // m is UNCHANGED — that's the point

    update_by_pointer(&m, 0.05, 1.2, 1.8);
    std::cout << "After update_by_pointer: " << format_metrics(m) << "\n";
    // m IS changed

    update_by_reference(m, 0.03, 0.95, 2.1);
    std::cout << "After update_by_reference: " << format_metrics(m) << "\n";
}
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Expecting `update_by_value` to change the caller | It won't — always use `&` or ref for out-params |
| Not checking pointer for `nullptr` | Always guard: `if (!m) return;` |
| Taking `const T*` but trying to modify | Remove `const` if mutation is needed |
