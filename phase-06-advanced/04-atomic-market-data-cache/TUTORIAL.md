# Tutorial — Atomic Market Data Cache

## Step 1 — Why Atomics?

Without synchronisation, two threads reading and writing a `double` simultaneously is **undefined behaviour** — the CPU may write 4 bytes, context-switch, then write the other 4 bytes.

`std::atomic<double>` guarantees that each read or write is **indivisible**.

```cpp
#include <atomic>
std::atomic<double> price{0.0};

// Thread A (publisher)
price.store(175.32, std::memory_order_release);

// Thread B (subscriber)
double p = price.load(std::memory_order_acquire);
```

> **Docs:** https://en.cppreference.com/w/cpp/atomic/atomic

---

## Step 2 — Memory Ordering

C++ gives you fine-grained control over how memory operations are ordered across threads:

| Order | Meaning | Use Case |
|-------|---------|----------|
| `relaxed` | No ordering guarantees | Counters (only the atomicity matters) |
| `release` | All prior writes visible before this store | Publishing a completed struct |
| `acquire` | All subsequent reads see the released writes | Reading a published struct |
| `seq_cst` | Globally consistent total order (default) | Simplest; slowest |

```cpp
// Publisher
bid_.store(bid, std::memory_order_release);

// Subscriber
double b = bid_.load(std::memory_order_acquire);
// Guaranteed: sees the publisher's release or later
```

> **Docs:** https://en.cppreference.com/w/cpp/atomic/memory_order

---

## Step 3 — PriceTick Struct

```cpp
struct PriceTick {
    std::atomic<double> bid{0.0};
    std::atomic<double> ask{0.0};
    std::atomic<double> last{0.0};
    std::atomic<bool>   valid{false};

    // Non-copyable — atomics cannot be copied
    PriceTick() = default;
    PriceTick(const PriceTick&) = delete;
    PriceTick& operator=(const PriceTick&) = delete;
};
```

> **Docs:** https://en.cppreference.com/w/cpp/atomic/atomic/atomic

---

## Step 4 — The Cache Class

```cpp
class MarketDataCache {
public:
    struct Snapshot { double bid; double ask; double last; bool valid; };

    bool update(std::string_view symbol, double bid, double ask, double last);
    Snapshot read(std::string_view symbol) const;
    long long update_count() const {
        return count_.load(std::memory_order_relaxed);
    }

private:
    static constexpr int MAX_SYMBOLS = 16;
    std::string  symbols_[MAX_SYMBOLS];
    PriceTick    ticks_[MAX_SYMBOLS];
    int          size_{0};
    std::atomic<long long> count_{0};
};
```

The `update()` method finds or inserts the symbol, stores the prices, marks `valid = true`, and increments `count_` with `relaxed` ordering (we only need atomicity, not ordering, for a counter).

---

## Step 5 — Compare-and-Swap for Optimistic Update

Sometimes you only want to update the price if it has changed beyond a threshold. CAS lets you do this lock-free:

```cpp
double current = bid_.load(std::memory_order_relaxed);
while (!bid_.compare_exchange_weak(current, new_bid,
                                    std::memory_order_release,
                                    std::memory_order_relaxed)) {
    // current was reloaded with the actual value; loop and retry
}
```

`compare_exchange_weak` returns `false` and reloads `current` if another thread changed `bid_` between your load and CAS.

> **Docs:** https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange

---

## Step 6 — Publisher + Reader Threads

```cpp
#include <thread>
#include <chrono>

MarketDataCache cache;

// Publisher
std::jthread publisher([&](std::stop_token st) {
    while (!st.stop_requested()) {
        cache.update("AAPL", 174.90, 175.10, 175.00);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
});

// Subscriber (runs on main thread)
for (int i = 0; i < 5; ++i) {
    auto snap = cache.read("AAPL");
    std::cout << "bid=" << snap.bid << " ask=" << snap.ask << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
// publisher's destructor calls request_stop() + join()
```

`std::jthread` automatically joins in its destructor and supports cooperative cancellation via `std::stop_token`.

> **Docs:** https://en.cppreference.com/w/cpp/thread/jthread

---

## Step 7 — ThreadSanitizer

ThreadSanitizer catches data races that mutexes and atomics are supposed to prevent. To enable it:

```bash
cmake -S . -B build -DCMAKE_CXX_FLAGS="-fsanitize=thread -g"
cmake --build build && ./build/atomic_cache
```

If you see any `WARNING: ThreadSanitizer: data race`, you have a bug.

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Using `relaxed` for both store and load on a flag | Pair `release` store with `acquire` load |
| Copying `std::atomic<T>` | Delete copy constructor / use pointer/reference |
| Assuming `seq_cst` is always safe | It is, but it has performance cost — choose the weakest ordering that is correct |
| Forgetting to initialise atomics | Use brace-initialisation: `std::atomic<int> x{0};` |
