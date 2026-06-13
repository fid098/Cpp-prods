#pragma once
#include <string>
#include <utility>

class Position {
public:
    Position(std::string symbol, int qty, double price)
        : symbol_{std::move(symbol)}, qty_{qty}, price_{price} {}

    double      value()  const { return static_cast<double>(qty_) * price_; }
    std::string symbol() const { return symbol_; }
    int         qty()    const { return qty_; }
    double      price()  const { return price_; }
    void set_price(double p) { price_ = p; }

private:
    std::string symbol_;
    int         qty_{0};
    double      price_{0.0};
};
