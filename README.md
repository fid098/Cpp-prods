# C++23 Quant Finance Curriculum

A project-based curriculum for learning modern C++ through quantitative finance. Every project is production-flavoured, portfolio-worthy, and builds directly on the one before it.

## Philosophy

- **No toy problems.** Every project solves a real finance problem: pricing, risk, data pipelines, order books.
- **Concepts first, jargon second.** Each phase introduces one cluster of C++ ideas and applies them where they matter most.
- **Portfolio-ready from day one.** README, tests, and working code — every project can go straight on GitHub or a CV.
- **The final project is the boss fight.** Phase 8 capstone combines everything into a full quant platform.

## Prerequisites

- A C++ compiler that supports C++23: GCC 13+, Clang 17+, or MSVC 19.35+
- CMake 3.25+
- Git

```bash
# Verify your setup:
g++ --version       # or clang++ --version
cmake --version
```

## How to Work Through This Curriculum

1. Open the project folder for your current phase and project number.
2. Read **README.md** for requirements and goals.
3. Read **TUTORIAL.md** for a step-by-step walkthrough and concept explanations.
4. Implement the project (start with the skeleton in `src/main.cpp` and `include/`).
5. Run the tests: `cmake -S . -B build && cmake --build build && cd build && ctest --output-on-failure`
6. All tests green? Polish, commit, push to GitHub.

## Curriculum Map

| Phase | Focus | Projects |
|-------|-------|----------|
| [1 — Fundamentals](phase-01-fundamentals/) | syntax, variables, functions, I/O | 4 |
| [2 — Memory](phase-02-memory/) | pointers, stack/heap, references | 4 |
| [3 — OOP](phase-03-oop/) | classes, inheritance, polymorphism | 4 |
| [4 — STL](phase-04-stl/) | containers, iterators, algorithms | 4 |
| [5 — Modern C++](phase-05-modern-cpp/) | smart pointers, move semantics, lambdas, concepts | 5 |
| [6 — Advanced](phase-06-advanced/) | templates, exceptions, threads, atomics | 4 |
| [7 — Tooling](phase-07-tooling/) | CMake, debuggers, sanitizers, GoogleTest | 4 |
| [8 — Capstones](phase-08-capstones/) | full systems, end-to-end C++23 | 5 |

## Quick Reference — Key Docs

| Topic | Link |
|-------|------|
| cppreference (your bible) | https://en.cppreference.com |
| CMake docs | https://cmake.org/cmake/help/latest/ |
| GoogleTest primer | https://google.github.io/googletest/primer.html |
| Compiler Explorer | https://godbolt.org |
| C++ Core Guidelines | https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines |

## Build System

Each project is self-contained with its own `CMakeLists.txt`. GoogleTest is pulled in via `FetchContent` — no manual install needed.

```bash
cd phase-01-fundamentals/01-cli-pnl-calculator
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/pnl_calculator          # run the app
cd build && ctest --output-on-failure  # run tests
```
