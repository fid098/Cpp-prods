# 03 — Sanitizer Risk Lab

## What You'll Build

A **risk calculation module** that contains four classes of undefined behaviour. You'll learn to use AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) to catch these automatically — the same tools used by Google, LLVM, and every serious C++ shop.

## Bug Map

| # | File | Sanitizer | Type |
|---|------|-----------|------|
| 1 | `var_calculator.hpp` | ASan | Stack buffer overflow — VaR loop writes past array end |
| 2 | `portfolio_snapshot.hpp` | ASan | Use-after-free — raw pointer used after `delete` |
| 3 | `position_sizer.hpp` | UBSan | Signed integer overflow — `int` position * multiplier |
| 4 | `risk_ratios.hpp` | UBSan | Division by zero — Sharpe ratio with zero std dev |

## Learning Objectives

| Sanitizer | What It Catches |
|-----------|-----------------|
| `-fsanitize=address` | Buffer overflows, use-after-free, heap/stack leaks |
| `-fsanitize=undefined` | Integer overflow, null dereference, div-by-zero, bad shifts |
| `-fsanitize=thread` | Data races, lock-order violations |
| Combined | Use both simultaneously for maximum coverage |

## Key Resources

| Topic | Link |
|-------|------|
| AddressSanitizer | https://clang.llvm.org/docs/AddressSanitizer.html |
| UBSan | https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html |
| ThreadSanitizer | https://clang.llvm.org/docs/ThreadSanitizer.html |
| GCC sanitizers | https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html |

## Build & Run

```bash
# Normal build — may or may not crash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build && ./build/risk_lab

# ASan — catches memory errors
cmake -S . -B build_asan -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
cmake --build build_asan && ./build_asan/risk_lab

# UBSan — catches undefined behaviour
cmake -S . -B build_ubsan -DCMAKE_CXX_FLAGS="-fsanitize=undefined -g"
cmake --build build_ubsan && ./build_ubsan/risk_lab

# Both at once (most thorough)
cmake -S . -B build_san -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -g"
cmake --build build_san && ./build_san/risk_lab

# Tests (pass after all bugs fixed)
ctest --test-dir build -V
```
