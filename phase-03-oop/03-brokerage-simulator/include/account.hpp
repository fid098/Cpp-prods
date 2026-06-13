#pragma once
#include <string>
#include <algorithm>

struct Order {
    enum class Status { PENDING, FILLED, REJECTED };
    std::string symbol;
    int         qty{0};
    double      price{0.0};
    bool        is_buy{true};
    Status      status{Status::PENDING};
};

class Account {
public:
    explicit Account(double initial_balance) : balance_{initial_balance} {}
    virtual ~Account() = default;

    double balance() const { return balance_; }

    // Returns the commission for a trade of the given notional value.
    virtual double fee(double notional) const = 0;

    // Submits an order. Returns true if accepted and fills immediately.
    virtual bool submit_order(const std::string& sym, int qty,
                               double price, bool is_buy) = 0;

protected:
    double balance_;
};

class CashAccount : public Account {
public:
    using Account::Account;

    double fee(double notional) const override {
        // TODO: return std::max(1.0, notional * 0.0001)
        (void)notional;
        return 0.0;
    }

    bool submit_order(const std::string& sym, int qty,
                       double price, bool is_buy) override {
        // TODO: compute notional, deduct or credit (minus fee), check balance
        (void)sym; (void)qty; (void)price; (void)is_buy;
        return false;
    }
};

class MarginAccount : public Account {
public:
    MarginAccount(double balance, double max_leverage)
        : Account{balance}, max_leverage_{max_leverage} {}

    double fee(double notional) const override {
        // TODO: return std::max(0.5, notional * 0.00005)
        (void)notional;
        return 0.0;
    }

    bool submit_order(const std::string& sym, int qty,
                       double price, bool is_buy) override {
        // TODO: allow buying on margin up to max_leverage_ * balance_
        (void)sym; (void)qty; (void)price; (void)is_buy;
        return false;
    }

    double max_leverage() const { return max_leverage_; }

private:
    double max_leverage_{2.0};
};
