#include <iostream>
#include <iomanip>
#include "pricing_graph.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Pricing Graph ===\n\n";

    // shared_ptr — both ForwardNode and SpotObserver share ownership
    auto spot = std::make_shared<SpotNode>("AAPL", 175.0);
    std::cout << "SpotNode price:   " << spot->compute() << "\n";

    ForwardNode fwd{spot, 0.05, 0.25};
    std::cout << "ForwardNode price: " << fwd.compute() << "\n";

    // weak_ptr observer — does not extend spot's lifetime
    SpotObserver obs{spot};
    std::cout << "Observer sees:    " << obs.try_observe() << "\n";

    // unique_ptr — sole ownership transferred via std::move
    std::unique_ptr<SpotNode> owned = std::make_unique<SpotNode>("MSFT", 290.0);
    std::cout << "\nUniquePtr MSFT:   " << owned->compute() << "\n";
    auto new_owner = std::move(owned);  // transfer ownership
    std::cout << "After move, owned is null: " << (owned == nullptr ? "YES" : "NO") << "\n";
    std::cout << "New owner price:  " << new_owner->compute() << "\n";

    // Demonstrate weak_ptr after source destroyed
    {
        auto temporary = std::make_shared<SpotNode>("TEMP", 50.0);
        SpotObserver temp_obs{temporary};
        std::cout << "\nTemp observer sees: " << temp_obs.try_observe() << "\n";
        // temporary goes out of scope here
        std::cout << "After destroy, observer sees: " << temp_obs.try_observe() << " (−1 = expired)\n";
    }

    return 0;
}
