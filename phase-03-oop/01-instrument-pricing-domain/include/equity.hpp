#pragma once
#include "instrument.hpp"
#include <utility>

class Equity : public Instrument
{
public:

    //default constructor
    Equity() {}

    // Done: implement parameterised constructor using member initialiser list
    Equity(std::string symbol, int quantity, double market_price)
        : symbol_{std::move(symbol)}, quantity_{quantity}, market_price_{market_price}
    {}

    // Done: implement price() → quantity_ * market_price_
    double price() const override;
    std::string name() const override { return symbol_; }

    int quantity() const { return quantity_; }
    double market_price() const { return market_price_; }
    void set_price(double p) { market_price_ = p; }

private:
    std::string symbol_;
    int quantity_{0};
    double market_price_{0.0};
};

inline double Equity::price() const
{
    // Done: return quantity_ * market_price_
    double result{0.0};

    result = quantity_ * market_price_;
    return result;
}
