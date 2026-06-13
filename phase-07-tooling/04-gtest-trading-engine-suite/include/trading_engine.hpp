#pragma once
#include "risk_checker.hpp"
#include <map>
#include <vector>
#include <stdexcept>
#include <cassert>

enum class Side { BUY, SELL };

struct Order {
    int    id;
    Side   side;
    int    quantity;
    double price;
};

class OrderBook {
public:
    void add_order(int id, Side side, int qty, double price) {
        if (qty <= 0) throw std::invalid_argument("qty must be positive");
        if (price <= 0.0) throw std::invalid_argument("price must be positive");
        orders_[id] = Order{id, side, qty, price};
        if (side == Side::BUY)  bids_[price] += qty;
        else                    asks_[price] += qty;
    }

    void cancel_order(int id) {
        auto it = orders_.find(id);
        if (it == orders_.end()) throw std::runtime_error("order not found");
        auto& o = it->second;
        if (o.side == Side::BUY)  bids_[o.price] -= o.quantity;
        else                       asks_[o.price] -= o.quantity;
        orders_.erase(it);
    }

    double best_bid() const {
        if (bids_.empty()) throw std::runtime_error("no bids");
        return bids_.rbegin()->first;
    }

    double best_ask() const {
        if (asks_.empty()) throw std::runtime_error("no asks");
        return asks_.begin()->first;
    }

    std::vector<double> all_ask_prices() const {
        std::vector<double> out;
        for (auto& [p, q] : asks_) if (q > 0) out.push_back(p);
        return out;
    }

    bool has_order(int id) const { return orders_.count(id) > 0; }
    int  order_count()     const { return static_cast<int>(orders_.size()); }

private:
    std::map<double, int>  bids_;
    std::map<double, int>  asks_;
    std::map<int, Order>   orders_;
};

inline bool validate_order(int qty, double price) {
    return qty > 0 && price > 0.0;
}

inline void enforce_risk_limit(long long notional) {
    if (notional > 5'000'000) {
        // In production this would be logged and rejected — here we crash to
        // demonstrate EXPECT_DEATH in the test suite.
        assert(false && "risk limit breached");
    }
}

class TradingEngine {
public:
    explicit TradingEngine(IRiskChecker* risk) : risk_{risk} {}

    bool submit_order(int qty, double price) {
        if (!risk_->check_order(qty, price)) return false;
        book_.add_order(next_id_++, Side::BUY, qty, price);
        return true;
    }

    const OrderBook& book() const { return book_; }

private:
    IRiskChecker* risk_;
    OrderBook     book_;
    int           next_id_{1};
};
