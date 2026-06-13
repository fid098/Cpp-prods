#pragma once
#include <memory>
#include <string>
#include <cmath>
#include <utility>

class PricingNode {
public:
    virtual double compute() const = 0;
    virtual ~PricingNode() = default;
};

class SpotNode : public PricingNode {
public:
    SpotNode(std::string sym, double price)
        : symbol_{std::move(sym)}, spot_{price} {}
    double compute() const override { return spot_; }
    void   update(double p) { spot_ = p; }
    const std::string& symbol() const { return symbol_; }
private:
    std::string symbol_;
    double      spot_;
};

class ForwardNode : public PricingNode {
public:
    ForwardNode(std::shared_ptr<SpotNode> spot, double rate, double T)
        : spot_{std::move(spot)}, rate_{rate}, T_{T} {}

    // F = S * exp(r * T)
    double compute() const override {
        // TODO: return spot_->compute() * std::exp(rate_ * T_)
        (void)rate_; (void)T_;
        return 0.0;
    }

private:
    std::shared_ptr<SpotNode> spot_;
    double rate_, T_;
};

// Observer node — does NOT extend the lifetime of the SpotNode it watches
class SpotObserver {
public:
    explicit SpotObserver(std::weak_ptr<SpotNode> weak) : weak_{std::move(weak)} {}

    // Returns current spot price, or -1.0 if the SpotNode was destroyed.
    double try_observe() const {
        if (auto sp = weak_.lock()) return sp->compute();
        return -1.0;
    }

private:
    std::weak_ptr<SpotNode> weak_;
};
