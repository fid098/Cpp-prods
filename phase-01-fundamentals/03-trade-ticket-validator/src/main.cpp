#include <iostream>
#include <string>
#include "trade_validator.hpp"

int main() {
    std::cout << "Trade Ticket Validator\n";
    std::cout << "Format: <symbol> <side> <qty> <type> <price>  |  quit to exit\n\n";

    TradeTicket t;
    while (std::cin >> t.symbol) {
        if (t.symbol == "quit") break;

        std::cin >> t.side >> t.qty >> t.type >> t.price;

        // TODO: call validate(t), print ACCEPTED or REJECTED: <reason>
    }

    return 0;
}
