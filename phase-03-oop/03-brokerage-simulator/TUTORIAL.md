# Tutorial — Brokerage Simulator

The two previous projects were about **composition** (a `Portfolio` *has* `Position`s). This one is about **inheritance**: a `CashAccount` *is an* `Account`, and so is a `MarginAccount`. The payoff is **polymorphism**, where one piece of code (`process_orders`) works correctly for every account type, including ones that don't exist yet.

---

## Step 1 — Decide What's Shared and What Varies

Before writing a hierarchy, sort every behaviour into one of two groups:

| Same for every account → base class | Different per account → `virtual` |
|---|---|
| holding a balance | the fee schedule |
| validating symbol/qty/price | whether an order is allowed |
| how a fill moves the balance (`fill_order`) | the display name (`kind`) |

The base class owns the left column. The right column becomes `virtual` functions that each subclass overrides. If you get this split wrong, you end up copy-pasting logic into every subclass, or forcing subclasses to work around a base class that does too much.

---

## Step 2 — The Abstract Base Class

```cpp
class Account {
public:
    explicit Account(double initial_balance) : balance_{initial_balance} {}
    virtual ~Account() = default;

    double balance() const { return balance_; }

    virtual double fee(double notional) const = 0;              // pure virtual
    virtual bool   submit_order(const std::string& sym, int qty,
                                double price, bool is_buy) = 0; // pure virtual
    virtual std::string kind() const = 0;

protected:
    static bool is_valid(const std::string& sym, int qty, double price);
    double buy_cost(double notional) const { return notional + fee(notional); }
    void   fill_order(double notional, bool is_buy);
    double balance_;
};
```

There are four things to notice here.

**`virtual`** tells the compiler to decide which version to call at *run time*, based on the object's real type. Each polymorphic object carries a hidden pointer to a **vtable**, a per-class table of function pointers. `account.fee(x)` looks up `fee` in that table.

**`= 0`** makes a function *pure virtual*. Any class with a pure virtual function is **abstract**: `Account a{100};` is a compile error. That's what we want here, because a "generic account" with no fee schedule makes no sense.

**`virtual ~Account()`** matters whenever someone might `delete` a derived object through a base pointer:

```cpp
Account* a = new MarginAccount{1000, 2};
delete a;   // without a virtual dtor → only ~Account runs → undefined behaviour
```

Rule of thumb: **if a class has any virtual function, give it a virtual destructor.**

**`explicit`** stops `Account a = 5000.0;` from silently converting a `double` into an account.

> **Docs:** https://en.cppreference.com/w/cpp/language/virtual · https://en.cppreference.com/w/cpp/language/abstract_class

---

## Step 3 — `public` vs `protected` vs `private`

| Access | Class itself | Subclasses | Outside code (`main`, tests) |
|---|:-:|:-:|:-:|
| `public` | ✓ | ✓ | ✓ |
| `protected` | ✓ | ✓ | ✗ |
| `private` | ✓ | ✗ | ✗ |

- `balance()` is **public**: anyone may *read* the balance.
- `balance_`, `fill_order`, and `buy_cost` are **protected**: subclasses need them to implement trades, but `main()` must not be able to write `cash.balance_ = 1e9;`.
- `MarginAccount::equity_` and `max_leverage_` are **private**: only `MarginAccount` touches them.

**Design note:** `protected` *data* is a mild smell in large codebases. Every subclass can break the balance invariant. A safer design keeps `balance_` private and exposes only protected *functions* like `fill_order()`. We use protected data here because the README asks you to practice it. Try the private version as an exercise.

> **Docs:** https://en.cppreference.com/w/cpp/language/access

---

## Step 4 — `fill_order`: Shared Logic That Calls Virtual Logic

```cpp
void fill_order(double notional, bool is_buy) {
    if (is_buy) balance_ -= buy_cost(notional);          // notional + fee
    else        balance_ += notional - fee(notional);
}
```

`fill_order` is **non-virtual** because the arithmetic of a fill is the same everywhere. Inside it, though, `fee(notional)` *is* virtual. The base class writes the algorithm, and each subclass plugs in the variable part. This is the **Template Method** pattern, one of the most common uses of inheritance.

> ⚠️ This works in a normal member function. It would **not** work in the `Account` *constructor*. While the base constructor runs, the object is still only an `Account`, so virtual calls do not reach the derived override. (Calling a pure virtual there is undefined behaviour.)

---

## Step 5 — `CashAccount`: `override`, `final`, Inherited Constructors

```cpp
class CashAccount final : public Account {
public:
    using Account::Account;   // reuse Account(double) as our constructor

    double fee(double notional) const override {
        return std::max(1.0, notional * 0.0001);   // 0.01%, min $1
    }

    bool submit_order(const std::string& sym, int qty,
                      double price, bool is_buy) override {
        if (!is_valid(sym, qty, price)) return false;
        const double notional = qty * price;
        if (is_buy && buy_cost(notional) > balance_) return false;  // can't afford
        fill_order(notional, is_buy);
        return true;
    }
    std::string kind() const override { return "Cash"; }
};
```

- **`: public Account`** is *public inheritance*, meaning "is-a". A `CashAccount` can be used anywhere an `Account&` is expected.
- **`override`** asks the compiler to check that this really overrides a base virtual function. Without it, a typo such as `double fee(double n)` (missing `const`) silently creates a *new, unrelated* function, and the base version keeps getting called. With `override`, that typo is a compile error. **Always write it.**
- **`final`** on the class means nothing can derive from `CashAccount`. It documents intent and lets the compiler skip the vtable lookup when it knows the exact type.
- **`using Account::Account;`** inherits the base constructors, so you don't have to write `CashAccount(double b) : Account{b} {}`.
- **Check first, then mutate.** Every rejection path returns *before* `fill_order`, so a rejected order never changes the balance. The test `InsufficientFundsRejected` checks this.

---

## Step 6 — `MarginAccount`: Extra State, Extra API

```cpp
class MarginAccount final : public Account {
public:
    MarginAccount(double balance, double max_leverage)
        : Account{balance}, equity_{balance},
          max_leverage_{std::max(1.0, max_leverage)} {}

    double credit_limit() const { return (max_leverage_ - 1.0) * equity_; }
    double buying_power() const { return balance_ + credit_limit(); }
    // submit_order: reject if buy_cost(notional) > buying_power()
private:
    double equity_;
    double max_leverage_;
};
```

**Margin model.** With leverage *L* and equity *E*, you may control up to *L × E* of stock, which means borrowing up to *(L − 1) × E*. Borrowed money shows up as a negative cash balance:

```
deposit $10,000 at 2x → credit limit $10,000
BUY TSLA 20 @ 700 → cost 14,000.70 → balance −4,000.70   (borrowed $4,000.70)
buying power left    = −4,000.70 + 10,000 = 5,999.30
BUY NVDA 10 @ 800 → cost 8,000.50 > 5,999.30 → REJECTED
```

A `MarginAccount` with leverage 1 has a credit limit of 0, so it behaves exactly like a cash account. That's a good sanity check, and it's one of the tests.

**Constructor order:** the base class (`Account{balance}`) is always constructed first, then the members in the order they're *declared* (not the order you list them in the initializer list). `-Wall` warns if the two orders differ.

*Simplification:* real brokers recompute equity from live position prices. We don't track positions here, so equity stays at the deposit.

---

## Step 7 — Polymorphic Dispatch: `process_orders`

```cpp
inline void process_orders(Account& account, std::vector<Order>& orders) {
    for (auto& order : orders) {
        const bool ok = account.submit_order(order.symbol, order.qty,
                                             order.price, order.is_buy);
        order.status        = ok ? Order::Status::FILLED : Order::Status::REJECTED;
        order.fee_paid      = ok ? account.fee(order.qty * order.price) : 0.0;
        order.balance_after = account.balance();
    }
}
```

This function knows nothing about cash or margin. Hand it a `CashAccount` and the cash rules run. Hand it a `MarginAccount` and the margin rules run. The test `SameOrdersDifferentOutcomes` sends the *same* order to both accounts and gets different results.

**Why `Account&` and not `Account`?** Passing a derived object *by value* to a base parameter **slices** it: only the `Account` part is copied, and the derived part (and its overrides) is thrown away. Here, `Account` is abstract, so taking it by value won't even compile. With a non-abstract base it compiles fine and quietly does the wrong thing. **Polymorphism only works through references or pointers.**

It's `inline` because it's defined in a header. Without `inline`, two `.cpp` files including `account.hpp` would each define it, and the linker would reject the duplicate (the One Definition Rule).

---

## Step 8 — Heterogeneous Collections and `dynamic_cast`

```cpp
std::vector<Account*> accounts = {&cash, &margin};
for (const Account* acc : accounts)
    std::cout << acc->kind() << " fee=$" << acc->fee(100000.0) << "\n";
```

One container holds different concrete types, because it stores *pointers to the base*. A `std::vector<Account>` can't do this: it would slice, and here it wouldn't compile. These pointers are **non-owning**: they point at stack objects, so there is nothing to `delete`. In Phase 5 you'll use `std::unique_ptr<Account>` for owning collections.

`max_leverage()` exists only on `MarginAccount`, so `acc->max_leverage()` doesn't compile. To get at it, ask the object what it really is:

```cpp
if (auto* m = dynamic_cast<MarginAccount*>(acc)) {
    std::cout << m->max_leverage();   // safe: acc really is a MarginAccount
} else {
    // acc is something else; dynamic_cast returned nullptr
}
```

| Cast | Checked at run time? | On failure |
|---|---|---|
| `dynamic_cast<D*>(base_ptr)` | ✓ (uses RTTI) | returns `nullptr` |
| `dynamic_cast<D&>(base_ref)` | ✓ | throws `std::bad_cast` |
| `static_cast<D*>(base_ptr)` | ✗ | **undefined behaviour** if you're wrong |

`dynamic_cast` requires the base to be *polymorphic* (to have at least one virtual function).

**Design note:** a chain of `if (dynamic_cast<A*>) ... else if (dynamic_cast<B*>) ...` is a sign the behaviour should have been a virtual function. `kind()` is a good example: instead of casting to find the type name, each class just reports it. Use `dynamic_cast` for *occasional* access to type-specific extras, not as a replacement for virtual dispatch.

> **Docs:** https://en.cppreference.com/w/cpp/language/dynamic_cast

---

## Step 9 — Testing a Hierarchy

The test file checks the design at three levels:

1. **Compile time**, with `static_assert(std::is_abstract_v<Account>)` and `std::has_virtual_destructor_v<Account>`. If someone breaks the design, the *build* fails.
2. **Each class on its own**: fee schedules, exact balances, rejections, invalid input, and the boundary case of a buy that costs *exactly* the buying power.
3. **Through the base**: `process_orders`, a `const Account&` calling `fee`, and `dynamic_cast` with both pointers and references.

```bash
cmake -S . -B build && cmake --build build && cd build && ctest --output-on-failure
```

---

## Common Pitfalls

| Mistake | Symptom | Fix |
|---|---|---|
| Forgetting `virtual` on the base method | Base version always runs | Mark it `virtual` in the base |
| Signature mismatch (missing `const`) | Override silently ignored | Always write `override` |
| No virtual destructor | UB on `delete base_ptr` | `virtual ~Base() = default;` |
| Passing/storing derived objects by base *value* | Slicing: derived behaviour lost | Use `Base&`, `Base*`, or smart pointers |
| Calling a virtual from the base constructor | Derived override not called | Move the call out of the constructor |
| Mutating state before validating | Rejected order still changes balance | Check everything, *then* `fill_order` |
| `static_cast` to downcast when unsure | UB on the wrong type | `dynamic_cast` and check for `nullptr` |

## Exercises

1. Make `balance_` **private** and keep the subclasses working with only protected functions.
2. Add a `PremiumCashAccount` with a flat $0.00 fee on trades over $50,000. How much existing code had to change? (Answer: none, and that's the point.)
3. Track positions per symbol so a cash account can't sell shares it doesn't own.
4. Add a margin call: when `borrowed() > credit_limit()` after prices move, reject all buys until the loan shrinks.
