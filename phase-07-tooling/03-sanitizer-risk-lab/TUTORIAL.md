# Tutorial — Sanitizer Risk Lab

## Step 1 — What Sanitizers Do

Sanitizers are compiler instrumentations that insert runtime checks into your binary. When a check fails, the program prints a detailed error message and aborts — far better than a silent crash or wrong result.

They are not for production — the performance overhead is 2–10x. They are for **development and CI**.

---

## Step 2 — AddressSanitizer

Enable with `-fsanitize=address` (GCC and Clang):

```bash
g++ -fsanitize=address -g -o prog prog.cpp
./prog
```

**Example ASan output:**

```
==12345==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7fff...
READ of size 8 at 0x7fff... thread T0
    #0 compute_var var_calculator.hpp:18
    #1 main main.cpp:12
```

The output gives you the exact file, line, and type of violation.

> **Docs:** https://clang.llvm.org/docs/AddressSanitizer.html

---

## Step 3 — UBSan

Enable with `-fsanitize=undefined`:

```bash
g++ -fsanitize=undefined -g -o prog prog.cpp
./prog
```

**Example UBSan output:**

```
position_sizer.hpp:25: runtime error: signed integer overflow:
2000000000 * 3 cannot be represented in type 'int'
```

Key UBSan checks enabled by default:
- `-fsanitize=signed-integer-overflow`
- `-fsanitize=null`
- `-fsanitize=zero`
- `-fsanitize=shift`
- `-fsanitize=vla-bound`

> **Docs:** https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html

---

## Bug 1 — Stack Buffer Overflow (var_calculator.hpp)

```cpp
// BUG: sorted[n] — one past end of array
double compute_var(const double* returns, int n, double confidence) {
    double sorted[n];
    for (int i = 0; i <= n; ++i) sorted[i] = returns[i];  // WRITES sorted[n]!
    ...
}
```

**Fix:** `i < n`

---

## Bug 2 — Use-After-Free (portfolio_snapshot.hpp)

```cpp
// BUG: snapshot is deleted but then accessed via stale pointer
void refresh(PortfolioSnapshot* snapshot) {
    delete snapshot;
    snapshot->timestamp = std::time(nullptr);  // USE AFTER FREE!
}
```

**Fix:** Set `snapshot = nullptr` before returning, or update before deleting.

---

## Bug 3 — Signed Integer Overflow (position_sizer.hpp)

```cpp
int size_position(int notional, int multiplier) {
    return notional * multiplier;  // overflows when result > INT_MAX
}
```

**Fix:** Use `long long`.

---

## Bug 4 — Division by Zero (risk_ratios.hpp)

```cpp
double sharpe_ratio(double mean_return, double std_dev) {
    return mean_return / std_dev;  // UB when std_dev == 0.0
}
```

**Fix:** Guard: `if (std_dev == 0.0) return 0.0;`

---

## Clean Run

After fixing all bugs:

```bash
cmake -S . -B build_san -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -g"
cmake --build build_san
./build_san/risk_lab
# No sanitizer output — clean!
ctest --test-dir build -V
# [ PASSED ] 7 tests.
```
