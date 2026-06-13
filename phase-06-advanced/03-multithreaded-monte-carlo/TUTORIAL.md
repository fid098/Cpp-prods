# Tutorial — Multithreaded Monte Carlo

## Step 1 — Single-threaded Monte Carlo

```cpp
double monte_carlo_call(double S, double K, double r, double sigma, double T,
                         int paths, std::mt19937& rng) {
    std::normal_distribution<double> norm{0.0, 1.0};
    double sum_payoff = 0.0;
    for (int i = 0; i < paths; ++i) {
        double Z  = norm(rng);
        double ST = S * std::exp((r - 0.5*sigma*sigma)*T + sigma*std::sqrt(T)*Z);
        sum_payoff += std::max(ST - K, 0.0);
    }
    return std::exp(-r * T) * (sum_payoff / paths);
}
```

This is the baseline.

---

## Step 2 — Thread-Local RNG (Important!)

Each thread needs its own random number generator — sharing one requires a mutex and is slow:

```cpp
thread_local std::mt19937 thread_rng{std::random_device{}()};
```

`thread_local` creates one instance per thread, initialised independently.

> **Docs:** https://en.cppreference.com/w/cpp/language/storage_duration

---

## Step 3 — Parallel with std::async

```cpp
#include <future>
#include <thread>

double parallel_mc(double S, double K, double r, double sigma, double T,
                    int total_paths, int num_threads) {
    int paths_per_thread = total_paths / num_threads;

    std::vector<std::future<double>> futures;
    for (int t = 0; t < num_threads; ++t) {
        futures.push_back(
            std::async(std::launch::async, [=]() {
                thread_local std::mt19937 rng{std::random_device{}()};
                return monte_carlo_call(S, K, r, sigma, T, paths_per_thread, rng);
            })
        );
    }

    double total = 0.0;
    for (auto& f : futures) total += f.get();
    return total / num_threads;
}
```

`std::async(std::launch::async, ...)` runs the lambda in a new thread and returns a `std::future<double>`. Calling `.get()` blocks until the result is ready.

> **Docs:** https://en.cppreference.com/w/cpp/thread/async

---

## Step 4 — Mutex Example (for comparison)

```cpp
std::mutex mtx;
double shared_sum = 0.0;

auto worker = [&](int paths) {
    double local_sum = 0.0;
    // ... accumulate into local_sum without holding the lock ...
    std::lock_guard<std::mutex> lock{mtx};
    shared_sum += local_sum;   // only lock for the final addition
};
```

Holding the mutex for the entire computation kills parallelism. Compute locally, lock only for the write.

> **Docs:** https://en.cppreference.com/w/cpp/thread/mutex

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Shared RNG between threads without mutex | Use `thread_local` RNG — no lock needed |
| Calling `future.get()` in the same thread as the async launch in a loop | The loop serialises — all `async`s must be launched first, then all `get()`s |
| Data race on shared `double` | Either use `std::mutex` or `std::atomic<double>` |
