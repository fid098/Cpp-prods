#pragma once

// Interface for the risk checker — used for GMock injection.
class IRiskChecker {
public:
    virtual ~IRiskChecker() = default;
    virtual bool check_order(int qty, double price) const = 0;
};

// Production implementation
class SimpleRiskChecker : public IRiskChecker {
public:
    explicit SimpleRiskChecker(double max_notional = 1'000'000.0)
        : max_notional_{max_notional} {}

    bool check_order(int qty, double price) const override {
        return qty > 0 && price > 0.0 && (qty * price) <= max_notional_;
    }

private:
    double max_notional_;
};
