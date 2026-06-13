#include <gtest/gtest.h>
#include "order_entry.hpp"

TEST(TotalQuantity, SumsAllEntries) {
    OrderEntry entries[3] = {{99.5, 200, 'B'}, {99.25, 300, 'B'}, {99.0, 100, 'B'}};
    EXPECT_EQ(total_quantity(entries, 3), 600);
}

TEST(TotalQuantity, SingleEntry) {
    OrderEntry e{100.0, 50, 'A'};
    EXPECT_EQ(total_quantity(&e, 1), 50);
}

TEST(TotalQuantity, ZeroEntries) {
    OrderEntry e{100.0, 50, 'A'};
    EXPECT_EQ(total_quantity(&e, 0), 0);
}

TEST(BestPrice, FindsMaxPrice) {
    OrderEntry entries[3] = {{99.0, 100, 'B'}, {101.0, 50, 'A'}, {100.5, 200, 'A'}};
    const OrderEntry* best = best_price(entries, 3);
    ASSERT_NE(best, nullptr);
    EXPECT_DOUBLE_EQ(best->price, 101.0);
}

TEST(BestPrice, NullptrOnEmpty) {
    OrderEntry e{100.0, 50, 'A'};
    EXPECT_EQ(best_price(&e, 0), nullptr);
}

TEST(PointerArithmetic, AdvancesBySize) {
    OrderEntry entries[2] = {{1.0, 10, 'B'}, {2.0, 20, 'B'}};
    const OrderEntry* p0 = entries;
    const OrderEntry* p1 = entries + 1;
    // p1 should equal &entries[1]
    EXPECT_EQ(p1, &entries[1]);
    // byte distance should be sizeof(OrderEntry)
    std::ptrdiff_t diff = reinterpret_cast<const char*>(p1)
                        - reinterpret_cast<const char*>(p0);
    EXPECT_EQ(diff, static_cast<std::ptrdiff_t>(sizeof(OrderEntry)));
}
