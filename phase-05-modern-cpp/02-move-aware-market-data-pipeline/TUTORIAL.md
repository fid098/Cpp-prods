# Tutorial — Move-Aware Market Data Pipeline

## Step 1 — The Cost of Copying

```cpp
std::vector<Tick> process(std::vector<Tick> ticks) {  // COPY — expensive!
    // ...
    return ticks;
}
```

For 200,000 ticks × 24 bytes = ~5 MB, a copy takes microseconds. For a real-time system processing thousands of batches per second, this adds up.

---

## Step 2 — Move Semantics to the Rescue

```cpp
std::vector<Tick> process(std::vector<Tick> ticks) {  // ticks is moved in
    // ...
    return ticks;   // NRVO or move — no copy
}

// At the call site:
auto result = process(std::move(input_batch));  // moves, not copies
// input_batch is now in a valid-but-empty state
```

`std::move` is just a cast to rvalue reference — it says "I no longer care about this value".

> **Docs:** https://en.cppreference.com/w/cpp/utility/move

---

## Step 3 — TickBatch with Move Support

```cpp
struct TickBatch {
    std::vector<Tick> ticks;
    std::string source;
    long long   timestamp{0};

    // Default move constructor/assignment are generated automatically
    // because std::vector already supports move.
    TickBatch(TickBatch&&) = default;
    TickBatch& operator=(TickBatch&&) = default;

    // Disable copy (this batch should never be accidentally copied)
    TickBatch(const TickBatch&) = delete;
    TickBatch& operator=(const TickBatch&) = delete;
};
```

Marking copy operations as `= delete` makes accidental copies a compile error.

---

## Step 4 — Pipeline Stage

```cpp
TickBatch validate_stage(TickBatch batch) {
    auto& t = batch.ticks;
    // Remove ticks with price <= 0
    t.erase(std::remove_if(t.begin(), t.end(),
        [](const Tick& tick) { return tick.price <= 0.0; }),
        t.end());
    return batch;  // move-eligible (NRVO)
}
```

```cpp
// In main:
auto raw     = ingest_stage("AAPL_feed.bin");
auto valid   = validate_stage(std::move(raw));    // move
auto enriched= enrich_stage(std::move(valid));    // move
publish(std::move(enriched));                     // move
```

---

## Step 5 — Lvalue vs Rvalue

```cpp
TickBatch a{...};          // a is an lvalue (has a name, persists)
process(a);                // COPY if process takes by value
process(std::move(a));     // MOVE — a is left in valid-but-empty state

process(TickBatch{...});   // the temporary IS an rvalue — no std::move needed
```

> **Docs:** https://en.cppreference.com/w/cpp/language/value_category
