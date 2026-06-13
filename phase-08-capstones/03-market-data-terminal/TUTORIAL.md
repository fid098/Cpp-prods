# Tutorial — Market Data Terminal

## Step 1 — ANSI Console Tricks

```cpp
// Clear screen and move cursor to top-left
std::cout << "\033[2J\033[H";

// Move cursor to row r, col c
std::cout << "\033[" << r << ";" << c << "H";

// Colors
std::cout << "\033[32m" << "green text" << "\033[0m";  // 32=green, 31=red, 33=yellow
```

> **Docs:** https://en.wikipedia.org/wiki/ANSI_escape_code

---

## Step 2 — Price Store with Atomics

Each instrument has its own atomic slots for bid, ask, last, and change direction:

```cpp
struct InstrumentData {
    std::atomic<double> bid{0.0};
    std::atomic<double> ask{0.0};
    std::atomic<double> last{0.0};
    std::atomic<double> prev_last{0.0};
    std::atomic<bool>   valid{false};
};
```

Publisher writes with `memory_order_release`, renderer reads with `memory_order_acquire`.

---

## Step 3 — jthread with stop_token

`std::jthread` automatically joins on destruction and can be stopped cooperatively:

```cpp
std::jthread publisher([](std::stop_token st, PriceStore& store) {
    std::mt19937 rng{std::random_device{}()};
    while (!st.stop_requested()) {
        // update prices
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}, std::ref(store));

// publisher stops and joins here when it goes out of scope
```

> **Docs:** https://en.cppreference.com/w/cpp/thread/jthread

---

## Step 4 — Simulated Price Movement

Geometric Brownian Motion with a discrete step:

```
S(t+dt) = S(t) * exp((μ - σ²/2)*dt + σ*√dt*Z)
```

For a simple simulation with small dt:

```cpp
std::normal_distribution<double> norm{0.0, 1.0};
double dt = 1.0 / (252.0 * 6.5 * 3600.0);  // 1 second in trading years
double drift = 0.0;
double vol   = 0.25;
double Z     = norm(rng);
price *= std::exp((drift - 0.5*vol*vol)*dt + vol*std::sqrt(dt)*Z);
```

---

## Step 5 — Renderer Loop

```cpp
std::jthread renderer([](std::stop_token st, const PriceStore& store) {
    while (!st.stop_requested()) {
        std::cout << "\033[H";  // move to top — cheaper than full clear
        render_table(store);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}, std::cref(store));
```

Using `\033[H` to move to top instead of `\033[2J\033[H` (clear + home) avoids screen flicker.

---

## Step 6 — Change Indicators

```cpp
double prev = data.prev_last.load(std::memory_order_acquire);
double curr = data.last.load(std::memory_order_acquire);

std::string arrow;
const char* color;
if (curr > prev)      { arrow = "↑"; color = "\033[32m"; }  // green
else if (curr < prev) { arrow = "↓"; color = "\033[31m"; }  // red
else                  { arrow = "="; color = "\033[33m"; }   // yellow

std::cout << color << arrow << "\033[0m";
```
