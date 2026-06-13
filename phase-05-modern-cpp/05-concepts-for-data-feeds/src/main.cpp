#include <iostream>
#include "feed_concepts.hpp"

int main() {
    std::cout << "=== Concepts for Data Feeds ===\n\n";

    TickFeed feed{"AAPL", {100, 102, 101, 105, 98, 107, 103, 110, 108, 115}};
    MomentumStrategy strat{3, 0.02};

    int signals = count_signals(feed, strat);
    std::cout << "Signals generated: " << signals << "\n";

    return 0;
}
