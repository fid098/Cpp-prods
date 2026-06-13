# 04 — Leak Hunt: Portfolio Cache

> Phase 2 · Memory | C++23 | Diagnose and repair memory leaks in a deliberately broken cache

## What You'll Build

A portfolio cache that **intentionally contains memory bugs** — leaks, double-frees, and a dangling pointer. Your job is to find and fix every bug. This simulates real code review work and introduces memory sanitizers.

```
$ ./build/portfolio_cache
=== Portfolio Cache ===
Loaded 3 positions.
AAPL  qty=100  cost=150.00
MSFT  qty=200  cost=290.00
TSLA  qty=50   cost=700.00
Total value: $217000.00

# Without the fix:
$ valgrind ./build/portfolio_cache
==1234== LEAK SUMMARY:
==1234==    definitely lost: 72 bytes in 3 blocks   ← you will fix these
```

## Learning Objectives

- Read code and identify missing `delete`/`delete[]`
- Understand double-free undefined behaviour
- Understand dangling pointers and how to nullify after delete
- Use Address Sanitizer (ASan) to detect leaks at compile time
- Set `pointer = nullptr` after freeing

## Project Structure

```
04-leak-hunt-portfolio-cache/
├── include/
│   └── portfolio_cache.hpp   ← buggy cache code (fix it)
├── src/
│   └── main.cpp
├── tests/
│   └── test_portfolio_cache.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Bugs to Find and Fix

The code has **four intentional bugs**. Hints (not spoilers):

1. A `new` allocation whose `delete` was forgotten
2. A `delete[]` called twice on the same pointer
3. A pointer used after `delete` (use-after-free)
4. A pointer returned from a function that points to a now-destroyed local

Find them by reading the code and running under ASan. Comments in the header mark each bug location with `// BUG:`.

## Build with Address Sanitizer

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=ON
cmake --build build
./build/portfolio_cache     # ASan will report any memory errors
```

## Key Resources

| Concept | Link |
|---------|------|
| Address Sanitizer | https://clang.llvm.org/docs/AddressSanitizer.html |
| Valgrind | https://valgrind.org/docs/manual/quick-start.html |
| delete / delete[] | https://en.cppreference.com/w/cpp/language/delete |
| nullptr | https://en.cppreference.com/w/cpp/language/nullptr |
