#include <iostream>
#include <iomanip>
#include "tick_buffer.hpp"

int main() {
    std::cout << "=== Tick Buffer Lifetime Lab ===\n\n";
    std::cout << "sizeof(Tick) = " << sizeof(Tick) << " bytes\n\n";

    // Stack buffer
    std::cout << "--- Stack buffer (std::array<Tick,512>) ---\n";
    auto stack_buf = make_stack_buffer();
    std::cout << "  Address of stack_buf[0]: "
              << static_cast<void*>(stack_buf.data()) << "\n";
    std::cout << "  Total size: " << sizeof(Tick) * 512 << " bytes\n\n";

    // Heap buffer
    std::cout << "--- Heap buffer (new Tick[1000]) ---\n";
    const int N = 1000;
    Tick* heap_buf = make_heap_buffer(N);
    if (!heap_buf) {
        std::cout << "make_heap_buffer not yet implemented.\n";
        return 1;
    }
    fill_buffer(heap_buf, N);

    std::cout << "  Address of heap_buf[0]:  "
              << static_cast<void*>(heap_buf) << "\n";
    std::cout << "  Total size: " << sizeof(Tick) * N << " bytes\n";
    std::cout << "  Sum of volumes: " << sum_volume(heap_buf, N) << "\n\n";

    // TODO: show that stack address and heap address are far apart in virtual space

    delete[] heap_buf;
    std::cout << "Heap memory freed.\n";
    return 0;
}
