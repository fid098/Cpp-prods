# Tutorial — Order Book Memory Map

## Step 1 — What Is a Pointer?

A pointer is a variable whose value is a **memory address** — just a number saying "the real data lives at byte 0x55a3c2e0a2b0 in RAM".

```cpp
double price = 99.50;          // price lives at some address on the stack
double* ptr  = &price;         // ptr holds that address; & is "address-of"

std::cout << *ptr << "\n";     // * dereferences: gives the value AT the address
std::cout << static_cast<void*>(ptr) << "\n";  // prints the address itself
```

> **Docs:** https://en.cppreference.com/w/cpp/language/pointer

---

## Step 2 — Define OrderEntry

```cpp
struct OrderEntry {
    double price;
    int    quantity;
    char   side;    // 'B' = bid, 'A' = ask
};
```

Use `sizeof(OrderEntry)` to find how many bytes this takes:

```cpp
std::cout << "sizeof(OrderEntry) = " << sizeof(OrderEntry) << " bytes\n";
// Likely 16 or 24 depending on padding — experiment!
```

> **Docs:** https://en.cppreference.com/w/cpp/language/sizeof

---

## Step 3 — Heap Allocation with new[]

```cpp
// Allocate 3 OrderEntry objects on the heap; returns a pointer to the first one
OrderEntry* bids = new OrderEntry[3];

bids[0] = {99.50, 200, 'B'};
bids[1] = {99.25, 500, 'B'};
bids[2] = {99.00, 100, 'B'};
```

`new OrderEntry[3]` allocates enough contiguous bytes for 3 entries and returns the address of the first.

> **Docs:** https://en.cppreference.com/w/cpp/language/new

---

## Step 4 — Print Addresses and Pointer Arithmetic

```cpp
for (int i = 0; i < 3; ++i) {
    const OrderEntry* entry = bids + i;  // pointer arithmetic: bids + i
    std::cout << "  bid[" << i << "]"
              << "  addr=" << static_cast<const void*>(entry)
              << "  price=" << entry->price
              << "  qty="   << entry->quantity;
    if (i > 0)
        std::cout << "  (+" << sizeof(OrderEntry) << " bytes from bid[0])";
    std::cout << "\n";
}
```

`bids + i` moves the pointer forward by `i * sizeof(OrderEntry)` bytes. This is pointer arithmetic.

> **Docs:** https://en.cppreference.com/w/cpp/language/operator_arithmetic (pointer section)

---

## Step 5 — Always Free Heap Memory

```cpp
delete[] bids;   // must match new[]
delete[] asks;
```

Forgetting `delete[]` causes a **memory leak** — the OS reclaims it when the process exits, but in a long-running trading system it would accumulate.

> **Docs:** https://en.cppreference.com/w/cpp/language/delete

---

## Step 6 — Total Quantity via Pointer

Implement `total_bid_quantity` using a raw pointer loop:

```cpp
int total_bid_quantity(const OrderEntry* entries, int n) {
    int total = 0;
    for (const OrderEntry* p = entries; p < entries + n; ++p) {
        total += p->quantity;
    }
    return total;
}
```

`p->quantity` is shorthand for `(*p).quantity` — dereference then access the field.

---

## Step 7 — Stack vs Heap Comparison

Add this to main.cpp to see the contrast:

```cpp
OrderEntry stack_entry{100.0, 50, 'A'};       // on the stack
OrderEntry* heap_entry = new OrderEntry{100.0, 50, 'A'};  // on the heap

std::cout << "Stack addr: " << static_cast<void*>(&stack_entry) << "\n";
std::cout << "Heap  addr: " << static_cast<void*>(heap_entry)   << "\n";

delete heap_entry;
```

Stack addresses are typically higher (near the top of virtual memory on x86-64).

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Using `delete` instead of `delete[]` for array | Always match `new[]` with `delete[]` |
| Printing pointer with `(int*)` cast | Use `static_cast<void*>` or `static_cast<const void*>` |
| Using a deleted pointer | Set it to `nullptr` after `delete[]` |
