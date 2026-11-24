#pragma once
#include <algorithm>
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// Order — a plain data record (a "struct" = class whose members default to
// public). It carries the request (symbol/qty/price/side) and, after
// processing, the result (status, fee charged, balance after the fill).
// ─────────────────────────────────────────────────────────────────────────────
struct Order {
    enum class Status { PENDING, FILLED, REJECTED };

    std::string symbol;
    int         qty{0};
    double      price{0.0};
    bool        is_buy{true};
    Status      status{Status::PENDING};

    // Filled in by process_orders() so a report can be printed afterwards.
    double fee_paid{0.0};
    double balance_after{0.0};
};

// ─────────────────────────────────────────────────────────────────────────────
// Account — ABSTRACT base class.
//
// It has at least one *pure virtual* function (`= 0`), so you cannot create an
// `Account` directly — only concrete subclasses. It defines the interface every
// account shares, and owns the state + bookkeeping that is identical for all
// of them (the balance and how a fill moves it).
// ─────────────────────────────────────────────────────────────────────────────
class Account {
public:
    explicit Account(double initial_balance) : balance_{initial_balance} {}

    // A class meant to be used through a base pointer MUST have a virtual
    // destructor. Otherwise `delete base_ptr;` only runs ~Account() and skips
    // the derived destructor → undefined behaviour.
    virtual ~Account() = default;

    double balance() const { return balance_; }

    // Each account type has its own fee schedule → pure virtual.
    virtual double fee(double notional) const = 0;

    // Each account type has its own acceptance rule → pure virtual.
    // Returns true if the order was accepted and filled.
    virtual bool submit_order(const std::string& sym, int qty,
                              double price, bool is_buy) = 0;

    // Human-readable account type, used in reports.
    virtual std::string kind() const = 0;

protected:
    // `protected`: visible to subclasses, invisible to outside code.
    // Subclasses call these; `main()` and tests cannot.

    // Basic sanity checks shared by every account type.
    static bool is_valid(const std::string& sym, int qty, double price) {
        return !sym.empty() && qty > 0 && price > 0.0;
    }

    // Total cash a buy will consume: notional + commission.
    // Note: calling the virtual `fee()` here dispatches to the *derived*
    // class's fee schedule — the base doesn't need to know which one.
    double buy_cost(double notional) const { return notional + fee(notional); }

    // Applies an already-approved fill to the balance. Non-virtual on purpose:
    // the arithmetic of a fill is the same for every account, so subclasses
    // reuse it instead of copy-pasting it.
    void fill_order(double notional, bool is_buy) {
        if (is_buy) balance_ -= buy_cost(notional);
        else        balance_ += notional - fee(notional);
    }

    // `private` would be stricter (subclasses would need a setter), but the
    // README asks us to practice `protected` data. In larger codebases prefer
    // private data + protected functions like fill_order() above.
    double balance_;
};

// ─────────────────────────────────────────────────────────────────────────────
// CashAccount — no borrowing. A buy must be fully paid from the balance.
// Fee: 0.01% of notional, minimum $1.00.
// ─────────────────────────────────────────────────────────────────────────────
class CashAccount final : public Account {
public:
    using Account::Account;  // inherit Account(double) as our constructor

    double fee(double notional) const override {
        return std::max(1.0, notional * 0.0001);
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

// ─────────────────────────────────────────────────────────────────────────────
// MarginAccount — may borrow from the broker.
//
// Simplified model: the deposit is your equity. With leverage L you may
// control up to L × equity, i.e. borrow up to (L − 1) × equity. Borrowing
// shows up as a NEGATIVE balance. A buy is rejected if it would push the
// balance below −credit_limit().
//
//   deposit $5,000 at 2x → credit limit $5,000 → can buy up to $10,000.
//
// (Real brokers recompute equity from live position values; we have no
// positions here, so equity is fixed at the deposit.)
// Fee: 0.005% of notional, minimum $0.50.
// ─────────────────────────────────────────────────────────────────────────────
class MarginAccount final : public Account {
public:
    MarginAccount(double balance, double max_leverage)
        : Account{balance},
          equity_{balance},
          max_leverage_{std::max(1.0, max_leverage)} {}  // leverage < 1 is meaningless

    double fee(double notional) const override {
        return std::max(0.5, notional * 0.00005);
    }

    bool submit_order(const std::string& sym, int qty,
                      double price, bool is_buy) override {
        if (!is_valid(sym, qty, price)) return false;

        const double notional = qty * price;
        if (is_buy && buy_cost(notional) > buying_power()) return false;  // breach

        fill_order(notional, is_buy);
        return true;
    }

    std::string kind() const override { return "Margin"; }

    // ── MarginAccount-only API (NOT on Account) ──
    // To call these through an Account& you need dynamic_cast — see main.cpp.
    double max_leverage() const { return max_leverage_; }
    double credit_limit() const { return (max_leverage_ - 1.0) * equity_; }
    double buying_power() const { return balance_ + credit_limit(); }
    double borrowed()     const { return std::max(0.0, -balance_); }

private:
    // `private`: even a class deriving from MarginAccount can't touch these.
    // (`final` above means nobody can derive from it anyway.)
    double equity_;
    double max_leverage_;
};

// ─────────────────────────────────────────────────────────────────────────────
// process_orders — works for ANY Account subclass, present or future.
//
// It takes `Account&` (a reference to the base). Each call to submit_order()
// and fee() is resolved at RUN TIME via the object's vtable, so a CashAccount
// runs CashAccount's rules and a MarginAccount runs MarginAccount's.
//
// Taking `Account` by VALUE would not even compile (Account is abstract) —
// and for non-abstract bases it would "slice" off the derived part.
// ─────────────────────────────────────────────────────────────────────────────
inline void process_orders(Account& account, std::vector<Order>& orders) {
    for (auto& order : orders) {
        const bool ok = account.submit_order(order.symbol, order.qty,
                                             order.price, order.is_buy);
        order.status   = ok ? Order::Status::FILLED : Order::Status::REJECTED;
        order.fee_paid = ok ? account.fee(order.qty * order.price) : 0.0;
        order.balance_after = account.balance();
    }
}
