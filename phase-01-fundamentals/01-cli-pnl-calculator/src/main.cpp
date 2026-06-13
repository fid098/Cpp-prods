#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "pnl_calculator.hpp"

int main() {
    std::unordered_map<std::string, Position> positions;
    double total_realized_pnl = 0.0;

    std::cout << "=== PnL Calculator ===\n";
    std::cout << "Commands:\n";
    std::cout << "  BUY   <symbol> <qty> <price>\n";
    std::cout << "  SELL  <symbol> <qty> <price>\n";
    std::cout << "  PRICE <symbol> <price>\n";
    std::cout << "  LIST\n";
    std::cout << "  QUIT\n\n";

    std::cout << std::fixed << std::setprecision(2);

    std::string command;
    while (std::cin >> command) {

        if (command == "BUY") {
            std::string sym;
            int    qty   = 0;
            double price = 0.0;
            std::cin >> sym >> qty >> price;

            // TODO: If the symbol already exists, recalculate avg_cost using
            //       compute_avg_cost(), then add qty to quantity.
            //       If it's new, create a default Position and set its fields.
            (void)sym; (void)qty; (void)price;
        }

        else if (command == "SELL") {
            std::string sym;
            int    qty   = 0;
            double price = 0.0;
            std::cin >> sym >> qty >> price;

            // TODO: Check the symbol exists and qty <= position.quantity.
            //       Compute realized PnL with compute_realized_pnl().
            //       Subtract qty from position.quantity.
            //       Accumulate into total_realized_pnl.
            //       Print the realized PnL for this trade.
            (void)sym; (void)qty; (void)price;
        }

        else if (command == "PRICE") {
            std::string sym;
            double price = 0.0;
            std::cin >> sym >> price;

            // TODO: Update positions[sym].current_price.
            //       Print the updated unrealized PnL using compute_unrealized_pnl().
            (void)sym; (void)price;
        }

        else if (command == "LIST") {
            // TODO: Iterate over all positions and print symbol, qty, avg_cost,
            //       and unrealized PnL for each.
        }

        else if (command == "QUIT") {
            std::cout << "Total Realized PnL: " << total_realized_pnl << "\n";
            break;
        }

        else {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    return 0;
}
