# Tutorial — Instrument Pricing Domain

## Step 1 — Abstract Base Class

An **abstract class** defines the interface every instrument must implement. It cannot be instantiated directly.

```cpp
// include/instrument.hpp
#pragma once
#include <string>

class Instrument {
public:
    // Pure virtual: every subclass MUST provide an implementation
    virtual double      price() const = 0;
    virtual std::string name()  const = 0;

    // Virtual destructor: required whenever a class is used polymorphically
    virtual ~Instrument() = default;
};
```

The `= 0` makes `price()` a **pure virtual function** — calling it on the base class is impossible. Forgetting the virtual destructor is a classic C++ bug.

> **Docs:** https://en.cppreference.com/w/cpp/language/abstract_class
> **Docs:** https://en.cppreference.com/w/cpp/language/destructor#Virtual_destructors

---

## Step 2 — Equity Class

```cpp
// include/equity.hpp
#pragma once
#include "instrument.hpp"

class Equity : public Instrument {
public:
    Equity(std::string symbol, int quantity, double market_price)
        : symbol_{std::move(symbol)}, quantity_{quantity}, market_price_{market_price}
    {}

    double      price() const override { return quantity_ * market_price_; }
    std::string name()  const override { return symbol_; }

    // Getters (encapsulation — data is private)
    int    quantity()     const { return quantity_; }
    double market_price() const { return market_price_; }
    void   set_price(double p)  { market_price_ = p; }

private:
    std::string symbol_;
    int         quantity_;
    double      market_price_;
};
```

Key patterns:
- `: symbol_{std::move(symbol)}` — **member initialiser list** (preferred over assignment in the body)
- `override` — tells the compiler to verify this actually overrides a base virtual
- Private data with public getters — **encapsulation**

> **Docs:** https://en.cppreference.com/w/cpp/language/constructor

---

## Step 3 — Bond Class

Bond pricing sums the present value of each coupon payment plus the face value:

```
PV = Σ_{t=1}^{n}  coupon*face / (1+ytm)^t  +  face / (1+ytm)^n
```

```cpp
#include <cmath>  // std::pow

class Bond : public Instrument {
public:
    Bond(std::string symbol, double face, double coupon_rate,
         double ytm, int years)
        : symbol_{std::move(symbol)}, face_{face},
          coupon_rate_{coupon_rate}, ytm_{ytm}, years_{years}
    {}

    double price() const override {
        double pv = 0.0;
        double coupon = coupon_rate_ * face_;
        for (int t = 1; t <= years_; ++t)
            pv += coupon / std::pow(1.0 + ytm_, t);
        pv += face_ / std::pow(1.0 + ytm_, years_);
        return pv;
    }

    std::string name() const override { return symbol_; }

private:
    std::string symbol_;
    double face_, coupon_rate_, ytm_;
    int    years_;
};
```

> **Docs:** https://en.cppreference.com/w/cpp/numeric/math/pow

---

## Step 4 — Polymorphic print_sheet

```cpp
void print_sheet(const Instrument& inst) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << inst.name() << "  price=" << inst.price() << "\n";
}
```

`print_sheet` does not know whether `inst` is an `Equity`, `Bond`, or `EuropeanOption`. The runtime dispatches to the right `price()` via the **vtable**. This is polymorphism.

---

## Step 5 — Black-Scholes for EuropeanOption (Stretch)

For a European call:
```
d1 = (ln(S/K) + (r + σ²/2)*T) / (σ*√T)
d2 = d1 - σ*√T
C  = S*N(d1) - K*e^{-rT}*N(d2)
```

Where `N` is the cumulative normal distribution. Use `std::erfc` from `<cmath>` to implement it.

> Full derivation: https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Missing virtual destructor on base | Always add `virtual ~Instrument() = default;` |
| Forgetting `override` | The compiler will catch overrides that don't match the base signature |
| Calling `new Instrument` | Can't — it's abstract. Use `new Equity(...)` etc. |
