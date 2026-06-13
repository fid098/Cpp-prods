#pragma once
#include "events.hpp"
#include <deque>

class SimBroker {
public:
    explicit SimBroker(double commission_pct = 0.001)
        : commission_pct_{commission_pct} {}

    void on_order(const OrderEvent& order, std::deque<Event>& queue) {
        if (order.quantity == 0 || order.price <= 0.0) return;
        double commission = std::abs(order.quantity) * order.price * commission_pct_;
        queue.push_back(FillEvent{
            order.symbol,
            order.quantity,
            order.price,
            commission
        });
    }

private:
    double commission_pct_;
};
