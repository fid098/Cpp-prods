# Tutorial — Brokerage Simulator

## Step 1 — Inheritance Recap

```cpp
class Account {
public:
    explicit Account(double initial_balance)
        : balance_{initial_balance} {}

    double balance() const { return balance_; }

    // Subclasses provide their own fee schedule
    virtual double fee(double notional) const = 0;

    // Submits an order; returns true if accepted
    virtual bool submit_order(const std::string& sym, int qty, double price,
                               bool is_buy);

    virtual ~Account() = default;

protected:
    double balance_;   // accessible in subclasses but not outside
};
```

`protected` lets `CashAccount` and `MarginAccount` read and modify `balance_` directly without a public setter.

> **Docs:** https://en.cppreference.com/w/cpp/language/access

---

## Step 2 — CashAccount

```cpp
class CashAccount : public Account {
public:
    using Account::Account;  // inherit constructor

    double fee(double notional) const override {
        return std::max(1.0, notional * 0.0001);  // 0.01%, min $1
    }

    bool submit_order(const std::string& sym, int qty, double price,
                       bool is_buy) override {
        double notional = qty * price;
        double total    = is_buy ? notional + fee(notional) : 0.0;
        if (is_buy && total > balance_) return false;  // insufficient funds
        if (is_buy)  balance_ -= (notional + fee(notional));
        else         balance_ += (notional - fee(notional));
        return true;
    }
};
```

---

## Step 3 — The Order Struct

```cpp
struct Order {
    enum class Status { PENDING, FILLED, REJECTED };
    std::string symbol;
    int         qty;
    double      price;
    bool        is_buy;
    Status      status{Status::PENDING};
};
```

---

## Step 4 — Polymorphic Order Processing

```cpp
void process_orders(Account& account, std::vector<Order>& orders) {
    for (auto& order : orders) {
        bool ok = account.submit_order(order.symbol, order.qty,
                                        order.price, order.is_buy);
        order.status = ok ? Order::Status::FILLED : Order::Status::REJECTED;
    }
}
```

The same `process_orders` works for both `CashAccount` and `MarginAccount` because it operates on the `Account` base reference.

---

## Step 5 — dynamic_cast for Safe Downcasting

When you need `MarginAccount`-specific behaviour through an `Account*`:

```cpp
Account* acc = new MarginAccount(10000.0, 2.0);

if (auto* margin = dynamic_cast<MarginAccount*>(acc)) {
    // only enters here if acc really is a MarginAccount
    margin->set_leverage(3.0);
}
```

> **Docs:** https://en.cppreference.com/w/cpp/language/dynamic_cast
