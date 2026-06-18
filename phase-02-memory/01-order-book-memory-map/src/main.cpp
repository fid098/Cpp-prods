#include <iostream>
#include <iomanip>
#include "order_entry.hpp"

int main()
{
    std::cout << "=== Order Book Memory Map ===\n\n";
    std::cout << "sizeof(OrderEntry) = " << sizeof(OrderEntry) << " bytes\n\n";

    // Done: allocate 3 bids on the heap with new[]
    OrderEntry *bids = new OrderEntry[3];
    bids[0] = {99.50, 200, 'B'};
    bids[1] = {99.25, 500, 'B'};
    bids[2] = {99.00, 100, 'B'};

    // Done: print address of each bid using static_cast<const void*>(bids + i)
    //       and show the byte offset from bids[0]
    for (std::size_t i{}; i < 3; i++)
    {
        std::cout << static_cast<const void *>(bids + i) << "\n";
        if (i > 0)
        {
            std::cout << " + " << (i * sizeof(OrderEntry)) << " bytes away from bids[0]" << "\n";
        }
    }

    // Done: allocate 2 asks on the heap
    OrderEntry *asks = new OrderEntry[2];
    asks[0] = {100.25, 300, 'A'};
    asks[1] = {100.50, 150, 'A'};
    // Done: print ask addresses
    for (std::size_t i{}; i < 2; i++)
    {
        std::cout << "ask address " << 1 + i << ": " << static_cast<const void *>(&asks[i]) << "\n";
    }
    // Done: demonstrate pointer arithmetic: show that bids + 1 == &bids[1]
    std::cout << "bids + 1 == &bids[1]: " << std::boolalpha << (bids + 1 == &bids[1]) << "\n";

    // Done: call total_quantity() and print the result
    std::cout << "The total quantity is: " << total_quantity(bids, 3) << "\n";

    // Extra: call best_price() and print result
    std::cout << "The best price is at this address: " << best_price(asks, 2) << "\n";

    // Done: delete[] bids; delete[] asks;
    delete[] bids;
    delete[] asks;

    std::cout << "\nMemory freed.\n";
    return 0;
}
