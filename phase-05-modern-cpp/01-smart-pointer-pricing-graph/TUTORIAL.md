# Tutorial — Smart Pointer Pricing Graph

## Step 1 — unique_ptr: Sole Ownership

```cpp
#include <memory>

// The factory owns its product exclusively
std::unique_ptr<Pricer> pricer = std::make_unique<BlackScholesPricer>(params);

pricer->price();   // use normally

// Can MOVE ownership:
std::unique_ptr<Pricer> new_owner = std::move(pricer);
// pricer is now nullptr — it no longer owns anything
```

Rule: if you wrote `new X`, write `std::make_unique<X>` instead.

> **Docs:** https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

---

## Step 2 — shared_ptr: Shared Ownership

```cpp
// SpotNode is shared between ForwardNode and OptionNode
auto spot = std::make_shared<SpotNode>("AAPL", 175.0);

ForwardNode fwd{spot, 0.05, 0.25};   // fwd holds a shared_ptr to spot
OptionNode  opt{spot, fwd_ptr, 0.2}; // opt also holds a shared_ptr to spot

// spot's reference count is 3 (spot, fwd, opt each hold one)
// spot is destroyed only when ALL three release it
```

> **Docs:** https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared

---

## Step 3 — weak_ptr: Breaking Cycles

If Node A holds a `shared_ptr` to Node B and Node B holds a `shared_ptr` to Node A, neither is ever destroyed (cycle). Use `weak_ptr` to break it:

```cpp
class Node {
    std::shared_ptr<Node> child_;   // strong ref — I own the child
    std::weak_ptr<Node>   parent_;  // weak ref — I observe the parent but don't own it
};
```

To use a `weak_ptr`:
```cpp
if (auto locked = parent_.lock()) {
    // locked is a shared_ptr — valid only inside this block
    locked->do_something();
}
// lock() returns nullptr if the object was already destroyed
```

> **Docs:** https://en.cppreference.com/w/cpp/memory/weak_ptr/lock

---

## Step 4 — Pricing Node Graph Structure

```cpp
class PricingNode {
public:
    virtual double compute() const = 0;
    virtual ~PricingNode() = default;
};

class SpotNode : public PricingNode {
public:
    SpotNode(std::string sym, double price)
        : symbol_{std::move(sym)}, spot_{price} {}
    double compute() const override { return spot_; }
    void update(double p) { spot_ = p; }
private:
    std::string symbol_;
    double      spot_;
};

class ForwardNode : public PricingNode {
public:
    ForwardNode(std::shared_ptr<SpotNode> spot, double rate, double T)
        : spot_{std::move(spot)}, rate_{rate}, T_{T} {}

    double compute() const override {
        return spot_->compute() * std::exp(rate_ * T_);
    }
private:
    std::shared_ptr<SpotNode> spot_;
    double rate_, T_;
};
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| `shared_ptr<T>(new T)` | Use `make_shared<T>` — single allocation, exception-safe |
| Storing raw `this` in a child | Use `enable_shared_from_this` or redesign |
| Locking a `weak_ptr` without checking | Always check `if (auto p = weak.lock())` |
| Keeping `unique_ptr` while trying to share it | Upgrade to `shared_ptr` if sharing is needed |
