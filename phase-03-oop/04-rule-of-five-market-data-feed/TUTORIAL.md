# Tutorial — Rule of Five: Market Data Feed

In Phase 2 you managed raw `new`/`delete` by hand and hunted leaks. This project puts that knowledge **inside a class**, so that ownership is handled automatically every time the object is created, copied, moved, or destroyed. That idea is called **RAII** (Resource Acquisition Is Initialization): the constructor acquires, the destructor releases, and users of the class never write `delete`.

---

## The Problem: The Compiler's Defaults Are Wrong Here

If you don't write them, the compiler generates all five special members, and each one simply copies or moves **member by member**. For `Tick* data_`, that copies the *pointer*, not the ticks.

```cpp
MarketDataFeed a{100};
MarketDataFeed b = a;   // default copy: b.data_ == a.data_  (SHALLOW)
b[0].price = 1.0;       // also changes a[0]: they share one buffer
}                       // ~b deletes the buffer, then ~a deletes it AGAIN → double free
```

And if there were no destructor at all, the buffer would never be freed: a leak.

**Rule of Five:** if a class needs a custom destructor (because it owns a resource), it almost certainly needs custom copy and move operations too. All five go together.

---

## Step 0 — Write Down the Invariant

Before writing any special member, state what must *always* be true about the object:

```
0 <= size_ <= capacity_
data_ == nullptr  if and only if  capacity_ == 0
this object is the ONLY owner of data_
```

Every function, including the moved-from state, must leave this true. Each special member below is just "how to keep the invariant while doing X".

The class funnels all memory work through two helpers: `allocate()` (the only `new[]`) and `release()` (the only `delete[]`). That makes it easy to reason about, and it's where the instrumentation counters live.

---

## Step 1 — Destructor

```cpp
~MarketDataFeed() { release(data_); }   // delete[] data_;
```

- It runs automatically at end of scope, on `delete`, and when a container erases the element.
- `delete[] nullptr` is a safe no-op, so a moved-from (empty) object can be destroyed safely.
- `new[]` must pair with `delete[]`, never with plain `delete`.
- Destructors are implicitly `noexcept`. Never let one throw.

---

## Step 2 — Copy Constructor (deep copy)

```cpp
MarketDataFeed(const MarketDataFeed& other)
    : data_{allocate(other.capacity_)},
      size_{other.size_},
      capacity_{other.capacity_}
{
    std::copy(other.data_, other.data_ + other.size_, data_);
}
```

The new object gets **its own buffer** containing **copies of the elements**. Afterwards the two feeds are completely independent (test: `CopyIsIndependent`).

If `allocate` throws, the object was never constructed, so its destructor never runs and nothing leaks.

---

## Step 3 — Copy Assignment, Done Safely

Assignment differs from construction because `*this` **already owns a buffer**. The order of operations matters.

### ❌ The tempting version (it's a bug)

```cpp
MarketDataFeed& operator=(const MarketDataFeed& other) {
    if (this == &other) return *this;
    delete[] data_;                    // old buffer gone…
    data_ = new Tick[other.capacity_]; // …and if this throws bad_alloc,
                                       // data_ still points at freed memory.
    ...                                // Later ~MarketDataFeed deletes it AGAIN.
}
```

### ✅ Allocate first, then release

```cpp
MarketDataFeed& operator=(const MarketDataFeed& other) {
    if (this == &other) return *this;           // self-assignment

    Tick* fresh = allocate(other.capacity_);     // 1. may throw → *this untouched
    std::copy(other.data_, other.data_ + other.size_, fresh);

    release(data_);                              // 2. can't throw
    data_     = fresh;                           // 3. commit
    size_     = other.size_;
    capacity_ = other.capacity_;
    return *this;                                // allows a = b = c
}
```

This gives the **strong exception guarantee**: either the assignment fully succeeds, or it throws and `*this` is exactly as it was. The general recipe is to **do everything that can fail first, then commit with operations that can't fail.**

**Why the self-assignment guard?** With the safe ordering, `a = a` would still *work*, just wastefully: it would allocate, copy, and free. The guard skips that. In the naive version, the guard was the only thing preventing disaster.

### Alternative: copy-and-swap

A popular idiom gets the strong guarantee and self-assignment safety for free:

```cpp
friend void swap(MarketDataFeed& a, MarketDataFeed& b) noexcept {
    std::swap(a.data_, b.data_);
    std::swap(a.size_, b.size_);
    std::swap(a.capacity_, b.capacity_);
}
MarketDataFeed& operator=(MarketDataFeed other) {  // BY VALUE: copy (or move) happens here
    swap(*this, other);                             // take its guts
    return *this;                                   // other's destructor frees our old buffer
}
```

It's elegant, but it always allocates, even when `*this` has enough capacity. Either approach is fine; just know both.

> **Docs:** https://en.cppreference.com/w/cpp/language/copy_assignment

---

## Step 4 — Move Constructor (steal)

```cpp
MarketDataFeed(MarketDataFeed&& other) noexcept
    : data_{std::exchange(other.data_, nullptr)},
      size_{std::exchange(other.size_, 0)},
      capacity_{std::exchange(other.capacity_, 0)} {}
```

**`&&` means rvalue reference.** It binds to temporaries (`MarketDataFeed{3}`, a function's return value) and to things you explicitly `std::move`. Either way, the caller is saying "I don't need this object's value any more", so instead of copying 1000 ticks, we take the pointer. That's O(1), with no allocation.

**`std::move` doesn't move anything.** It's just a cast to `&&` that *allows* the move constructor to be chosen.

**`std::exchange(x, v)`** sets `x = v` and returns x's *old* value. It's "steal and reset" in one expression.

**The moved-from object** must be *valid but unspecified*: safe to destroy, and safe to assign a new value to. Ours is a clean empty feed (capacity 0), and you can even reuse it (test: `MovedFromIsReusable`). Without nulling `other.data_`, both objects would own the buffer, and you'd be back to the double free.

> **Docs:** https://en.cppreference.com/w/cpp/language/move_constructor · https://en.cppreference.com/w/cpp/utility/exchange

---

## Step 5 — Move Assignment

```cpp
MarketDataFeed& operator=(MarketDataFeed&& other) noexcept {
    if (this == &other) return *this;     // a = std::move(a)
    release(data_);                       // free what we own…
    data_     = std::exchange(other.data_, nullptr);   // …then steal
    size_     = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
    return *this;
}
```

Here the guard is **essential**. Without it, `a = std::move(a)` frees a's buffer and then "steals" the dangling pointer back from itself.

Freeing before stealing is fine here, unlike in copy assignment, because nothing after `release` can throw.

---

## Step 6 — Why `noexcept` Matters (the demo proves it)

When a `std::vector` runs out of capacity, it allocates a bigger array and has to transfer the existing elements. If that transfer threw halfway, the vector would be left broken. So the vector checks the type:

- if the move constructor is `noexcept`, it **moves** the elements (cheap);
- otherwise it **copies** them, so a throw leaves the old array intact (expensive).

The demo pushes 5 feeds and reports `copies=0 moves=7`. Capacity grew 1 → 2 → 4 → 8, so 1 + 2 + 4 = 7 elements were moved. Remove `noexcept` from the move constructor and rerun: the copies show up, along with 7 extra buffer allocations. **Mark moves `noexcept` whenever they truly can't throw.**

The tests enforce this at compile time:

```cpp
static_assert(std::is_nothrow_move_constructible_v<MarketDataFeed>);
```

> **Docs:** https://en.cppreference.com/w/cpp/language/noexcept

---

## Step 7 — Proving It With Counters

You can't *see* a leak or a double free; at best the program crashes later. So the header counts what happens:

```cpp
struct FeedStats {
    static inline int allocations = 0, frees = 0, copies = 0, moves = 0;
    static int live_buffers() { return allocations - frees; }
};
```

`static inline` data members (C++17) are defined directly in the header, with one shared copy for the whole program.

The test fixture checks `live_buffers() == 0` in `TearDown()`, after **every** test. A forgotten `delete[]` in any special member makes every test that uses it fail. Real projects get the same effect from tools like AddressSanitizer and Valgrind (Phase 7).

---

## Step 8 — The Rule of Zero (where this is heading)

Writing all five correctly takes real care, as you've just seen. The modern advice is: **don't own raw resources in ordinary classes.** Let a member that already handles ownership correctly do it:

```cpp
class MarketDataFeed {
    std::vector<Tick> data_;   // vector already has a correct Rule of Five
    int capacity_;
    // write NONE of the five; the compiler's defaults are now correct
};
```

This is the **Rule of Zero**. You write the Rule of Five only in low-level classes whose whole job is managing one resource (like `std::vector`, `std::unique_ptr`, or a file/socket wrapper), and everything else composes them. Phase 5 does exactly this with smart pointers.

> **Docs:** https://en.cppreference.com/w/cpp/language/rule_of_three

---

## Common Pitfalls

| Mistake | Consequence | Fix |
|---|---|---|
| No destructor | Leak | `delete[] data_` in `~T()` |
| Default (shallow) copy | Two owners → double free | Deep-copy in copy ctor/assign |
| `delete[]` old buffer **before** allocating the new one | Dangling pointer + double free if `new` throws | Allocate first, then release (or copy-and-swap) |
| Not nulling the source in a move | Two owners → double free | `std::exchange(other.data_, nullptr)` |
| No self-check in move assignment | `a = std::move(a)` destroys a's data | `if (this == &other) return *this;` |
| Move ops not `noexcept` | `std::vector` silently copies on growth | Mark them `noexcept` |
| `delete` instead of `delete[]` | Undefined behaviour | Match `new[]` with `delete[]` |
| Forgetting `return *this;` | `a = b = c` breaks | Return `*this` by reference |

## Exercises

1. Remove `noexcept` from the move constructor, rerun the program, and explain the new numbers.
2. Rewrite copy assignment using copy-and-swap. Do all tests still pass? What happens to `SelfAssignSafe`'s address check, and why?
3. Make copy assignment **reuse** the existing buffer when `capacity_ >= other.size_` (no allocation). Which tests need changing?
4. Rewrite the class using `std::vector<Tick>` with zero special members (Rule of Zero), and keep the same public API.
