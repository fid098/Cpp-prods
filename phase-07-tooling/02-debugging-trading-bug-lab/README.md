# 02 — Debugging Trading Bug Lab

## What You'll Build

A deliberately **buggy trading system** that you diagnose using GDB (Linux/Mac) or the VS Code debugger (Windows). Four categories of bugs are hidden in the code — your job is to find and fix each one. This is how real-world debugging works: you get a crash report or a wrong answer and have to trace through the code systematically.

## Bug Map

| Bug | File | Type |
|-----|------|------|
| 1 | `pnl_tracker.hpp` | Off-by-one: loop reads past end of array |
| 2 | `pnl_tracker.hpp` | Integer overflow: `int` PnL on large positions |
| 3 | `order_manager.hpp` | Logic error: wrong comparison operator in fill check |
| 4 | `order_manager.hpp` | Null dereference: accessing unfound order |

> **Do not read the TUTORIAL until you have attempted each bug yourself!**

## Learning Objectives

| Tool | Skill |
|------|-------|
| GDB `break` / `run` / `next` / `print` | Set breakpoints and inspect state |
| GDB `backtrace` (`bt`) | Read a crash stack trace |
| GDB `watch` | Break when a variable changes |
| VS Code Debug panel | Equivalent GUI workflow |
| `-g` flag | Compile with debug symbols |
| UBSan | Catch UB (overflow, OOB) automatically |

## Key Resources

| Topic | Link |
|-------|------|
| GDB quickstart | https://www.sourceware.org/gdb/documentation/ |
| GDB cheatsheet | https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf |
| VS Code C++ debug | https://code.visualstudio.com/docs/cpp/cpp-debug |
| UBSan | https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html |

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/trading_bugs          # observe wrong output or crash

# Recompile with UBSan to catch undefined behaviour
cmake -S . -B build_san -DCMAKE_CXX_FLAGS="-fsanitize=undefined,address -g"
cmake --build build_san
./build_san/trading_bugs      # UBSan reports the exact line

# Run tests (they should fail until bugs are fixed)
ctest --test-dir build -V
```
