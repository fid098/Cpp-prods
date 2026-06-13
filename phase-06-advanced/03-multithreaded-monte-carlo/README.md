# 03 — Multithreaded Monte Carlo

> Phase 6 · Advanced | C++23 | Parallel option pricing engine with threads and futures

## What You'll Build

A Monte Carlo European option pricer that distributes simulation paths across CPU cores using `std::thread`, `std::future`, and `std::async`. Observe the speedup as you increase the thread count.

```
=== Multithreaded Monte Carlo Option Pricer ===
Pricing AAPL call  S=175  K=180  r=5%  σ=20%  T=0.25yr

  Paths  Threads  Price    StdErr   Time(ms)
 100000        1  4.2341   0.0143      183
 100000        4  4.2289   0.0143       52   ← 3.5x speedup
1000000        8  4.2318   0.0045      190
```

## Learning Objectives

- Launch threads with `std::thread` and `std::async`
- Protect shared state with `std::mutex`
- Use `std::future` and `std::promise` to return results from threads
- Use `std::atomic<double>` for lock-free accumulation
- Understand race conditions and how to avoid them
- Understand thread-local random number generators

## Key Resources

| Concept | Link |
|---------|------|
| std::thread | https://en.cppreference.com/w/cpp/thread/thread |
| std::async | https://en.cppreference.com/w/cpp/thread/async |
| std::future | https://en.cppreference.com/w/cpp/thread/future |
| std::mutex | https://en.cppreference.com/w/cpp/thread/mutex |
| std::atomic | https://en.cppreference.com/w/cpp/atomic/atomic |
| thread_local | https://en.cppreference.com/w/cpp/language/storage_duration |
