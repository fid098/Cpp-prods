#include <iostream>
#include <iomanip>
#include "order_entry.hpp"

int main() {
    std::cout << "=== Order Book Memory Map ===\n\n";
    std::cout << "sizeof(OrderEntry) = " << sizeof(OrderEntry) << " bytes\n\n";

    // TODO: allocate 3 bids on the heap with new[]
    // OrderEntry* bids = new OrderEntry[3];
    // bids[0] = {99.50, 200, 'B'};
    // bids[1] = {99.25, 500, 'B'};
    // bids[2] = {99.00, 100, 'B'};

    // TODO: print address of each bid using static_cast<const void*>(bids + i)
    //       and show the byte offset from bids[0]

    // TODO: allocate 2 asks on the heap
    // TODO: print ask addresses
    // TODO: demonstrate pointer arithmetic: show that bids + 1 == &bids[1]

    // TODO: call total_quantity() and print the result

    // TODO: delete[] bids; delete[] asks;

    std::cout << "\nMemory freed.\n";
    return 0;
}
