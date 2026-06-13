# Tutorial — Debugging Trading Bug Lab

## GDB Quick Reference

```bash
gdb ./build/trading_bugs   # load program
(gdb) run                  # run until crash
(gdb) bt                   # backtrace — shows call stack
(gdb) frame 2              # switch to frame 2
(gdb) print variable       # print a variable's value
(gdb) break filename.cpp:42 # set breakpoint at line 42
(gdb) next                 # step over one line
(gdb) step                 # step into a function call
(gdb) watch expr           # break when expr changes
(gdb) quit
```

---

## Bug 1 — Off-by-One (pnl_tracker.hpp)

**Symptom:** Program occasionally outputs garbage values or crashes with `Segmentation fault`.

**How to find it:**

```bash
cmake -S . -B build_san -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
cmake --build build_san
./build_san/trading_bugs
# AddressSanitizer: heap-buffer-overflow on address ...
```

**What to look for:** A loop that iterates `for (int i = 0; i <= n; ...)` — the `<=` reads `prices[n]` which is one past the end of the array.

**Fix:** Change `i <= n` to `i < n`.

> **Docs:** https://en.cppreference.com/w/cpp/language/array

---

## Bug 2 — Integer Overflow (pnl_tracker.hpp)

**Symptom:** `compute_total_pnl()` returns a large negative number for a position of 1,000,000 shares at $500/share.

**How to find it:**

```bash
cmake -S . -B build_ub -DCMAKE_CXX_FLAGS="-fsanitize=undefined -g"
./build_ub/trading_bugs
# UBSan: runtime error: signed integer overflow: 1000000 * 500 cannot be represented in type 'int'
```

**Fix:** Change the accumulator type from `int` to `long long`.

> **Docs:** https://en.cppreference.com/w/cpp/types/integer

---

## Bug 3 — Logic Error (order_manager.hpp)

**Symptom:** `fill_order()` never fills an order even when there is enough quantity available.

**How to find it:** Set a breakpoint in `fill_order()`:

```bash
gdb ./build/trading_bugs
(gdb) break order_manager.hpp:45
(gdb) run
(gdb) print available
(gdb) print requested
# available=100, requested=100 — the condition "available < requested" is false
# but the original code uses "<" instead of "<="
```

**Fix:** Change `available < requested` to `available >= requested` (or restructure the condition).

---

## Bug 4 — Null Dereference (order_manager.hpp)

**Symptom:** `get_order_price()` crashes for any unknown order ID.

**How to find it:**

```bash
gdb ./build/trading_bugs
(gdb) run
# Program received signal SIGSEGV
(gdb) bt
# shows the crash is in get_order_price()
(gdb) print order_ptr
# $1 = 0x0  ← null pointer!
```

**Fix:** Add a null check: `if (!order_ptr) throw std::runtime_error("order not found")`.

> **Docs:** https://en.cppreference.com/w/cpp/language/nullptr

---

## Verification

Once all bugs are fixed, the test suite should pass:

```bash
ctest --test-dir build -V
# [  PASSED  ] 6 tests.
```
