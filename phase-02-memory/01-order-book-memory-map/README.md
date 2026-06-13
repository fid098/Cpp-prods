# 01 — Order Book Memory Map

> Phase 2 · Memory | C++23 | Visualise how bids and asks live in memory

## What You'll Build

A program that allocates a fixed array of bid and ask entries on the **heap**, then prints the raw memory addresses of each entry and shows pointer arithmetic. This demystifies what pointers actually are — they are just numbers that refer to memory locations.

```
=== Order Book Memory Map ===

Bids (heap-allocated):
  bid[0]  addr=0x55a3c2e0a2b0  price=99.50  qty=200
  bid[1]  addr=0x55a3c2e0a2c8  price=99.25  qty=500  (+24 bytes from bid[0])
  bid[2]  addr=0x55a3c2e0a2e0  price=99.00  qty=100  (+24 bytes from bid[1])

Asks (heap-allocated):
  ask[0]  addr=0x55a3c2e0a360  price=100.00  qty=300
  ask[1]  addr=0x55a3c2e0a378  price=100.25  qty=150  (+24 bytes from ask[0])

sizeof(OrderEntry) = 24 bytes
Pointer arithmetic: bids + 1 = 0x55a3c2e0a2c8  (matches bid[1] above)

Memory freed.
```

## Learning Objectives

- Understand that a pointer is just an integer address
- Use `new` and `delete[]` for heap allocation
- Print addresses with `static_cast<void*>(ptr)` and `std::cout`
- Perform pointer arithmetic: `ptr + n` advances by `n * sizeof(*ptr)` bytes
- Use `sizeof()` to inspect type sizes
- Understand the stack/heap distinction conceptually

## Project Structure

```
01-order-book-memory-map/
├── include/
│   └── order_entry.hpp    ← OrderEntry struct + display helpers
├── src/
│   └── main.cpp           ← allocate, inspect, free
├── tests/
│   └── test_order_entry.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. Define an `OrderEntry` struct: `price (double)`, `quantity (int)`, `side (char: 'B' or 'A')`
2. Allocate an array of 3 bids and 2 asks on the heap with `new`
3. Print each entry's address and byte offset from the first element
4. Demonstrate `sizeof(OrderEntry)` and pointer arithmetic
5. Free all allocated memory with `delete[]`
6. A function `total_bid_quantity(const OrderEntry*, int n)` sums quantities using pointer arithmetic

## Key Resources

| Concept | Link |
|---------|------|
| Pointers | https://en.cppreference.com/w/cpp/language/pointer |
| new / delete | https://en.cppreference.com/w/cpp/language/new |
| sizeof | https://en.cppreference.com/w/cpp/language/sizeof |
| Pointer arithmetic | https://en.cppreference.com/w/cpp/language/operator_arithmetic |
| static_cast | https://en.cppreference.com/w/cpp/language/static_cast |

## Stretch Goals

- Print a hex dump of the raw bytes of one `OrderEntry`
- Implement a `find_best_bid` function that returns a pointer to the highest-priced bid
- Show the stack address of a local variable vs the heap address of `new`
