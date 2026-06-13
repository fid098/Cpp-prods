#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "pnl_calculator.hpp"

int main()
{
    std::unordered_map<std::string, Position> positions;

    std::cout << "=== PnL Calculator ===\n";
    std::cout << "Commands:\n";
    std::cout << "  BUY   <symbol> <qty> <price>\n";
    std::cout << "  SELL  <symbol> <qty> <price>\n";
    std::cout << "  PRICE <symbol> <price>\n";
    std::cout << "  LIST\n";
    std::cout << "  QUIT\n\n";

    std::cout << std::fixed << std::setprecision(2);
    double total_realised_pnl = 0.0;

    std::string command;
    while (std::cin >> command)
    {

        if (command == "BUY")
        {
            std::string sym;
            int qty = 0;
            double price = 0.0;
            std::cin >> sym >> qty >> price;

            // DONE: If the symbol already exists, recalculate avg_cost using
            //       compute_avg_cost(), then add qty to quantity.
            //       If it's new, create a default Position and set its fields.
            Position &pos = positions[sym];

            if (pos.quantity > 0)
            {
                pos.avg_cost = compute_avg_cost(pos.quantity, pos.avg_cost, qty, price);
            }
            else
            {
                pos.symbol = sym;
                pos.avg_cost = price;
            }
            pos.quantity += qty;
        }

        else if (command == "SELL")
        {
            std::string sym;
            int qty = 0;
            double price = 0.0;
            std::cin >> sym >> qty >> price;

            // DONE: Check the symbol exists and qty <= position.quantity.
            //       Compute realized PnL with compute_realized_pnl().
            //       Subtract qty from position.quantity.
            //       Accumulate into total_realized_pnl.
            //       Print the realized PnL for this trade.

            if (positions.count(sym) == 0)
            {
                std::cout << "No position for " << sym << "\n";
            }
            else
            {
                Position &pos = positions[sym];
                if (qty <= pos.quantity)
                {
                    double realised_pnl = compute_realized_pnl(pos, qty, price);
                    pos.quantity -= qty;
                    total_realised_pnl += realised_pnl;
                    std::cout << std::fixed << std::setprecision(2) << realised_pnl << "\n";
                }
            }
        }

        else if (command == "PRICE")
        {
            std::string sym;
            double price = 0.0;
            std::cin >> sym >> price;

            // DONE: Update positions[sym].current_price.
            //       Print the updated unrealized PnL using compute_unrealized_pnl().
            if (positions.count(sym) == 0)
            {
                std::cout << "No position for " << sym << "\n";
            }
            else
            {
                Position &pos = positions[sym];
                pos.current_price = price;
                double unrealised_pnl = compute_unrealized_pnl(pos);
                std::cout << std::fixed << std::setprecision(2) << unrealised_pnl << "\n";
            }
        }

        else if (command == "LIST")
        {
            // DONE: Iterate over all positions and print symbol, qty, avg_cost,
            //       and unrealized PnL for each.
            for (auto &[key, value] : positions)
            {
                // key: sym, value: position structs
                std::cout << key << "\n";
                std::cout << value.quantity << "\n";
                std::cout << std::fixed << std::setprecision(2) << value.avg_cost << "\n";
                double unrealised_pnl = compute_unrealized_pnl(value);
                std::cout << std::fixed << std::setprecision(2) << unrealised_pnl << "\n";
            }
        }

        else if (command == "QUIT")
        {
            std::cout << "Total Realized PnL: " << total_realised_pnl << "\n";
            break;
        }

        else
        {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    return 0;
}
