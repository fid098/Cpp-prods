# Phase 7 — Tooling

**Topics covered:** CMake best practices, LLDB/GDB debuggers, Address Sanitizer, UBSan, compiler warning flags, GoogleTest fixtures, parameterised tests, death tests

Phase 7 is the craft layer. Writing correct C++ is necessary; shipping it confidently requires the right tools. By the end of this phase you will have a professional workflow that catches bugs before production does.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [cmake-professional-template](01-cmake-professional-template/) | Reusable C++23 project template | CMake targets, options, structure |
| 02 | [debugging-trading-bug-lab](02-debugging-trading-bug-lab/) | Debug a wrong-PnL defect with LLDB/GDB | breakpoints, watchpoints, call stacks |
| 03 | [sanitizer-risk-lab](03-sanitizer-risk-lab/) | Catch UB and memory bugs with sanitizers | ASan, UBSan, compiler flags |
| 04 | [gtest-trading-engine-suite](04-gtest-trading-engine-suite/) | Professional GoogleTest suite | fixtures, parameterised tests |

## Phase Resources

- https://cmake.org/cmake/help/latest/
- https://google.github.io/googletest/primer.html
- https://clang.llvm.org/docs/AddressSanitizer.html
- https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
- https://lldb.llvm.org/use/tutorial.html
