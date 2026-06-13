# 01 — Instrument Pricing Domain

> Phase 3 · OOP | C++23 | Model equities, bonds, and options with pricing behaviour

## What You'll Build

A class hierarchy that models three financial instruments — **Equity**, **Bond**, and **EuropeanOption** — each with its own `price()` method. A single `print_sheet()` function can price any instrument without knowing its concrete type.

```
=== Instrument Pricing Domain ===

Equity  AAPL  shares=100  market_price=175.00  value=17500.00
Bond    T10Y  face=1000   coupon=0.045  ytm=0.04  price=1033.78
Option  AAPL  call  strike=180  spot=175  T=0.25  price=4.23
```

## Learning Objectives

- Define a class with private data members and public member functions
- Write constructors (default, parameterised) and initialiser lists
- Use `virtual` functions and `override` keyword
- Understand abstract base classes with pure virtual functions (`= 0`)
- Understand `public` inheritance and the `is-a` relationship
- Use encapsulation: getters/setters vs direct access

## Project Structure

```
01-instrument-pricing-domain/
├── include/
│   ├── instrument.hpp        ← abstract base class
│   ├── equity.hpp
│   ├── bond.hpp
│   └── european_option.hpp
├── src/
│   └── main.cpp
├── tests/
│   └── test_instruments.cpp
├── CMakeLists.txt
├── README.md
└── TUTORIAL.md
```

## Requirements

1. **`Instrument`** — abstract base class with `virtual double price() const = 0` and `virtual std::string name() const = 0`
2. **`Equity`** — stores symbol, quantity, market\_price; `price()` returns `quantity * market_price`
3. **`Bond`** — face value, coupon rate, yield to maturity, years to maturity; `price()` computes present value of cash flows
4. **`EuropeanOption`** — Black-Scholes price for a European call or put
5. `print_sheet(const Instrument& i)` prints instrument name and price — works for any subtype

## Bond Pricing Formula

```
PV = Σ (coupon * face / (1+ytm)^t)  for t = 1 to n
   + face / (1+ytm)^n
```

## Key Resources

| Concept | Link |
|---------|------|
| Classes | https://en.cppreference.com/w/cpp/language/classes |
| Constructors | https://en.cppreference.com/w/cpp/language/constructor |
| Virtual functions | https://en.cppreference.com/w/cpp/language/virtual |
| Abstract classes | https://en.cppreference.com/w/cpp/language/abstract_class |
| override keyword | https://en.cppreference.com/w/cpp/language/override |
| std::pow | https://en.cppreference.com/w/cpp/numeric/math/pow |
