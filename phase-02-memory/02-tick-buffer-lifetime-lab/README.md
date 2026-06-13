# 02 — Tick Buffer Lifetime Lab

> Phase 2 · Memory | C++23 | Compare stack and heap storage for tick data

## What You'll Build

An experiment that stores the same batch of market ticks first on the **stack** (using a fixed `std::array`) and then on the **heap** (using `new`), benchmarks access time, and demonstrates the ownership rules that govern each.

```
=== Tick Buffer Lifetime Lab ===

Stack buffer (std::array<Tick, 1000>):
  Address of first tick: 0x7ffdb3a2e010
  Address of last  tick: 0x7ffdb3a2ee98
  Buffer size: 24000 bytes on the stack

Heap buffer (new Tick[1000]):
  Address of first tick: 0x55a1c3d00eb0
  Buffer size: 24000 bytes on the heap
  (heap address is far from stack address)

Stack vs Heap:
  Stack: automatically freed when the enclosing function returns
  Heap:  must be freed with delete[] — forgetting causes a leak

Memory freed.
```

## Learning Objectives

- Understand stack lifetime: automatic storage duration
- Understand heap lifetime: dynamic storage duration — you control it
- See the difference in memory addresses (stack is near top of virtual space)
- Understand why large buffers belong on the heap (stack is typically 1–8 MB)
- Practice scoped lifetime: wrapping heap allocation in a function

## Project Structure

```
02-tick-buffer-lifetime-lab/
├── include/
│   └── tick_buffer.hpp    ← Tick struct + TickBuffer functions
├── src/
│   └── main.cpp
├── tests/
│   └── test_tick_buffer.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. Define a `Tick` struct: `symbol[8]`, `price (double)`, `volume (int)`, `timestamp (long long)`
2. Implement `make_stack_buffer()` — returns a `std::array<Tick, 512>` (fits on the stack)
3. Implement `make_heap_buffer(int n)` — returns `new Tick[n]` (heap)
4. Implement `fill_buffer(Tick* buf, int n)` — populates ticks with synthetic data
5. Implement `sum_volume(const Tick* buf, int n)` — sums all volumes using a pointer loop
6. Show that a returned stack `std::array` is safe (copy out); a returned raw pointer to a local is not

## Key Resources

| Concept | Link |
|---------|------|
| Storage duration | https://en.cppreference.com/w/cpp/language/storage_duration |
| std::array | https://en.cppreference.com/w/cpp/container/array |
| Stack vs heap | https://en.cppreference.com/w/cpp/language/new |
| Dangling pointers | https://en.cppreference.com/w/cpp/language/pointer |
