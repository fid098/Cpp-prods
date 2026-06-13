# Tutorial — Tick Buffer Lifetime Lab

## Step 1 — Stack Storage Duration

Variables declared inside a function live on the **stack** and are automatically destroyed when the function returns:

```cpp
void stack_demo() {
    int x = 42;           // lives on the stack
    double prices[100];   // also on the stack — 800 bytes
    // x and prices are gone when stack_demo() returns
}
```

The stack is fast (just a pointer decrement) but limited in size (typically 1–8 MB). A 1,000,000-element array of `double` would overflow it.

> **Docs:** https://en.cppreference.com/w/cpp/language/storage_duration

---

## Step 2 — Heap Storage Duration

Memory allocated with `new` lives until you call `delete` (or the program exits):

```cpp
Tick* ticks = new Tick[1000];  // survives past the current scope
// ... use ticks ...
delete[] ticks;                // you decide when to free it
ticks = nullptr;               // good practice: prevent accidental reuse
```

The heap can handle large allocations — gigabytes if the OS allows.

---

## Step 3 — Define the Tick Struct

```cpp
struct Tick {
    char      symbol[8];    // 8-byte symbol string (e.g. "AAPL    ")
    double    price;
    int       volume;
    long long timestamp;    // Unix milliseconds
};
```

`sizeof(Tick)` will reveal how much space each tick takes (expect 24–32 bytes due to padding).

---

## Step 4 — Stack Buffer with std::array

```cpp
#include <array>

std::array<Tick, 512> make_stack_buffer() {
    std::array<Tick, 512> buf{};  // zero-initialised
    return buf;                   // returned by value (copy) — safe!
}
```

Returning a `std::array` by value is fine because the copy happens before the stack frame is destroyed. (The compiler often optimises this to a direct write via RVO.)

---

## Step 5 — The Dangling Pointer Trap (DO NOT DO THIS)

```cpp
// BAD — never do this:
Tick* dangerous() {
    Tick buf[4];          // on the stack
    return buf;           // BUG: returns pointer to local that will be destroyed!
}
```

After `dangerous()` returns, the stack frame is gone; the returned pointer points to garbage. Accessing it is **undefined behaviour**.

---

## Step 6 — fill_buffer and sum_volume

```cpp
inline void fill_buffer(Tick* buf, int n) {
    for (int i = 0; i < n; ++i) {
        // zero-init symbol
        for (int j = 0; j < 8; ++j) buf[i].symbol[j] = '\0';
        buf[i].price     = 100.0 + i * 0.01;
        buf[i].volume    = 100 + i;
        buf[i].timestamp = 1_000_000LL + i;
    }
}

inline long long sum_volume(const Tick* buf, int n) {
    long long total = 0;
    for (const Tick* p = buf; p < buf + n; ++p)
        total += p->volume;
    return total;
}
```

---

## Step 7 — Verify with Tests

The tests exercise `fill_buffer` and `sum_volume` with heap-allocated buffers (since heap allocation in tests is easier to control than stack-size limits).

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Returning a pointer to a local array | Return `std::array` by value or use the heap |
| Forgetting `delete[]` | Keep allocation and deallocation in the same scope |
| Using `new` but `delete` (no brackets) | Use `delete[]` for arrays, `delete` for single objects |
