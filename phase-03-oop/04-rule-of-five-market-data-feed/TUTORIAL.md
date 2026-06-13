# Tutorial — Rule of Five: Market Data Feed

## The Problem with Default Copy

```cpp
class Feed {
    Tick* data_;
    int   size_;
};

Feed a{...};
Feed b = a;   // SHALLOW copy — both a.data_ and b.data_ point to the SAME buffer!
// When a is destroyed: delete[] a.data_  → b.data_ is now dangling!
```

Any class that **owns** a raw resource (pointer, file handle, socket) needs custom special members.

---

## Step 1 — Destructor

```cpp
~MarketDataFeed() {
    delete[] data_;
}
```

---

## Step 2 — Copy Constructor (deep copy)

```cpp
MarketDataFeed(const MarketDataFeed& other)
    : data_{new Tick[other.capacity_]{}},
      size_{other.size_},
      capacity_{other.capacity_}
{
    std::copy(other.data_, other.data_ + other.size_, data_);
}
```

---

## Step 3 — Copy Assignment

```cpp
MarketDataFeed& operator=(const MarketDataFeed& other) {
    if (this == &other) return *this;   // self-assignment guard

    delete[] data_;                     // free old resource
    capacity_ = other.capacity_;
    size_     = other.size_;
    data_     = new Tick[capacity_]{};
    std::copy(other.data_, other.data_ + size_, data_);
    return *this;
}
```

---

## Step 4 — Move Constructor

```cpp
MarketDataFeed(MarketDataFeed&& other) noexcept
    : data_{other.data_},
      size_{other.size_},
      capacity_{other.capacity_}
{
    other.data_     = nullptr;  // leave source in valid empty state
    other.size_     = 0;
    other.capacity_ = 0;
}
```

`noexcept` is important — STL containers use move constructors more aggressively when they are marked noexcept.

> **Docs:** https://en.cppreference.com/w/cpp/language/noexcept

---

## Step 5 — Move Assignment

```cpp
MarketDataFeed& operator=(MarketDataFeed&& other) noexcept {
    if (this == &other) return *this;
    delete[] data_;
    data_     = std::exchange(other.data_,     nullptr);
    size_     = std::exchange(other.size_,     0);
    capacity_ = std::exchange(other.capacity_, 0);
    return *this;
}
```

`std::exchange(old, new)` sets `old` to `new` and returns the original value — a clean way to steal and nullify.

> **Docs:** https://en.cppreference.com/w/cpp/utility/exchange

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Forgetting self-assignment check | `if (this == &other) return *this;` |
| Not nulling the source in the move | Source destructor will `delete[] nullptr` — that is safe, but double-free without the null is UB |
| Copy-assigning before deleting the old buffer | Always `delete[]` the old data before allocating new |
