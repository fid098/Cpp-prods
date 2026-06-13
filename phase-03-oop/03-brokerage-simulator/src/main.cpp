#include <iostream>
#include <iomanip>
#include <vector>
#include "account.hpp"

void process_orders(Account& account, std::vector<Order>& orders) {
    for (auto& order : orders) {
        bool ok = account.submit_order(order.symbol, order.qty,
                                        order.price, order.is_buy);
        order.status = ok ? Order::Status::FILLED : Order::Status::REJECTED;
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Brokerage Simulator ===\n\n";

    CashAccount cash{10000.0};
    std::cout << "Cash account balance=$" << cash.balance() << "\n";

    std::vector<Order> orders = {
        {"AAPL", 100, 150.0, true},
        {"AAPL",  50, 160.0, false},
    };
    process_orders(cash, orders);
    for (const auto& o : orders) {
        std::cout << "  " << (o.is_buy ? "BUY" : "SELL")
                  << " " << o.symbol << " " << o.qty << "@" << o.price
                  << " status=" << (o.status == Order::Status::FILLED ? "FILLED" : "REJECTED")
                  << "  balance=$" << cash.balance() << "\n";
    }

    return 0;
}
