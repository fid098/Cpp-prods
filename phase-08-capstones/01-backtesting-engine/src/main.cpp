#include <iostream>
#include <iomanip>
#include <deque>
#include <variant>
#include "events.hpp"
#include "bar_feed.hpp"
#include "strategy.hpp"
#include "broker.hpp"
#include "portfolio.hpp"

void run_backtest(BarFeed& feed, Strategy& strategy,
                  SimBroker& broker, Portfolio& portfolio,
                  int trade_qty = 100) {
    std::deque<Event> queue;

    while (feed.has_next()) {
        queue.push_back(feed.next());

        while (!queue.empty()) {
            Event e = queue.front();
            queue.pop_front();

            std::visit([&](auto&& ev) {
                using T = std::decay_t<decltype(ev)>;
                if constexpr (std::is_same_v<T, BarEvent>) {
                    strategy.on_bar(ev, queue);
                    portfolio.on_bar(ev);
                } else if constexpr (std::is_same_v<T, SignalEvent>) {
                    if (ev.direction != 0) {
                        queue.push_back(OrderEvent{
                            ev.symbol,
                            ev.direction * trade_qty,
                            feed.last_close()
                        });
                    }
                } else if constexpr (std::is_same_v<T, OrderEvent>) {
                    broker.on_order(ev, queue);
                } else if constexpr (std::is_same_v<T, FillEvent>) {
                    portfolio.on_fill(ev);
                }
            }, e);
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Backtesting Engine ===\n\n";

    // Generate synthetic price series if no CSV provided
    std::vector<BarEvent> bars;
    if (argc >= 2) {
        BarFeed feed{std::string{argv[1]}};
        SMACrossover strategy{5, 20};
        SimBroker    broker;
        Portfolio    portfolio{100'000.0};
        run_backtest(feed, strategy, broker, portfolio);
        auto r = portfolio.report();
        std::cout << "Total return:   " << r.total_return * 100.0 << "%\n";
        std::cout << "Annualised Sharpe: " << r.annualised_sharpe << "\n";
        std::cout << "Max drawdown:   " << r.max_drawdown * 100.0 << "%\n";
        std::cout << "Total trades:   " << r.total_trades << "\n";
        std::cout << "Final equity:   $" << r.final_equity << "\n";
        return 0;
    }

    // Synthetic data demo
    double price = 100.0;
    for (int i = 0; i < 200; ++i) {
        double change = (i % 20 < 10) ? 0.5 : -0.3;
        price += change;
        bars.push_back({"SYNTH", price*0.99, price*1.01, price*0.98, price, 100000});
    }

    BarFeed   feed{std::move(bars)};
    SMACrossover strategy{5, 20};
    SimBroker    broker;
    Portfolio    portfolio{100'000.0};
    run_backtest(feed, strategy, broker, portfolio);

    auto r = portfolio.report();
    std::cout << "=== Backtest Results ===\n";
    std::cout << "Total return:      " << r.total_return * 100.0 << "%\n";
    std::cout << "Annualised Sharpe: " << r.annualised_sharpe << "\n";
    std::cout << "Max drawdown:      " << r.max_drawdown * 100.0 << "%\n";
    std::cout << "Total trades:      " << r.total_trades << "\n";
    std::cout << "Final equity:      $" << r.final_equity << "\n";

    return 0;
}
