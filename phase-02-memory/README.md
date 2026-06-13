# Phase 2 — Memory

**C++ concepts covered:** raw pointers, pointer arithmetic, addresses, stack vs heap, `new`/`delete`, references vs pointers, RAII basics, memory leaks, dangling pointers

Phase 2 goes under the hood. You will directly manipulate memory, understand the difference between stack and heap, and practise safe ownership discipline before moving to smart pointers in Phase 5.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [order-book-memory-map](01-order-book-memory-map/) | Visualise bids/asks in memory | pointers, addresses, pointer arithmetic |
| 02 | [tick-buffer-lifetime-lab](02-tick-buffer-lifetime-lab/) | Compare stack vs heap tick buffers | scope, lifetimes, stack vs heap |
| 03 | [risk-engine-reference-vs-pointer](03-risk-engine-reference-vs-pointer/) | Update risk metrics efficiently | pass by reference, pass by pointer |
| 04 | [leak-hunt-portfolio-cache](04-leak-hunt-portfolio-cache/) | Diagnose and repair a leaking cache | `new`/`delete`, dangling pointers, sanitizers |

## Phase Resources

- https://en.cppreference.com/w/cpp/language/pointer
- https://en.cppreference.com/w/cpp/language/reference
- https://en.cppreference.com/w/cpp/language/new
- https://en.cppreference.com/w/cpp/language/delete
- https://en.cppreference.com/w/cpp/memory
