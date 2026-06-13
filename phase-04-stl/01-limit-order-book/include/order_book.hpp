#pragma once
#include <map>
#include <unordered_map>
#include <queue>
#include <stdexcept>

struct Order {
    int    id{0};
    double price{0.0};
    int    qty{0};
    bool   is_bid{true};
};

struct PriceLevel {
    int             total_qty{0};
    std::queue<int> order_ids;
};

class OrderBook {
public:
    // Adds an order. Returns false if id already exists.
    bool add_order(int id, bool is_bid, double price, int qty);

    // Cancels an order by id. Returns false if not found.
    bool cancel_order(int id);

    // Returns the highest bid price. Returns 0.0 if no bids.
    double best_bid() const;

    // Returns the lowest ask price. Returns 0.0 if no asks.
    double best_ask() const;

    double spread() const { return best_ask() - best_bid(); }

    int bid_depth() const { return static_cast<int>(bids_.size()); }
    int ask_depth() const { return static_cast<int>(asks_.size()); }

    bool has_order(int id) const { return orders_.count(id) > 0; }

    void print_book() const;

private:
    std::map<double, PriceLevel>    bids_;
    std::map<double, PriceLevel>    asks_;
    std::unordered_map<int, Order>  orders_;
};

inline bool OrderBook::add_order(int id, bool is_bid, double price, int qty) {
    if (orders_.count(id)) return false;
    orders_[id] = {id, price, qty, is_bid};
    auto& book  = is_bid ? bids_ : asks_;
    auto& level = book[price];
    level.total_qty += qty;
    level.order_ids.push(id);
    return true;
}

inline bool OrderBook::cancel_order(int id) {
    auto it = orders_.find(id);
    if (it == orders_.end()) return false;
    const Order& o = it->second;
    auto& book = o.is_bid ? bids_ : asks_;
    auto lit   = book.find(o.price);
    if (lit != book.end()) {
        lit->second.total_qty -= o.qty;
        if (lit->second.total_qty <= 0) book.erase(lit);
    }
    orders_.erase(it);
    return true;
}

inline double OrderBook::best_bid() const {
    if (bids_.empty()) return 0.0;
    return bids_.rbegin()->first;
}

inline double OrderBook::best_ask() const {
    if (asks_.empty()) return 0.0;
    return asks_.begin()->first;
}

inline void OrderBook::print_book() const {
    // TODO: print asks low→high, then bids high→low
}
