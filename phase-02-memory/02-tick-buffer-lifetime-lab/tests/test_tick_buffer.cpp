#include <gtest/gtest.h>
#include "tick_buffer.hpp"

TEST(FillBuffer, SetsVolume) {
    Tick* buf = new Tick[5];
    fill_buffer(buf, 5);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(buf[i].volume, 100 + i);
    delete[] buf;
}

TEST(FillBuffer, SetsPrice) {
    Tick* buf = new Tick[3];
    fill_buffer(buf, 3);
    EXPECT_NEAR(buf[0].price, 100.00, 1e-9);
    EXPECT_NEAR(buf[1].price, 100.01, 1e-9);
    EXPECT_NEAR(buf[2].price, 100.02, 1e-9);
    delete[] buf;
}

TEST(SumVolume, CorrectSum) {
    Tick* buf = new Tick[4];
    fill_buffer(buf, 4);
    // volumes: 100, 101, 102, 103 → sum = 406
    EXPECT_EQ(sum_volume(buf, 4), 406);
    delete[] buf;
}

TEST(SumVolume, EmptyReturnsZero) {
    Tick buf[1]{};
    EXPECT_EQ(sum_volume(buf, 0), 0);
}

TEST(MakeHeapBuffer, ReturnsNonNull) {
    Tick* buf = make_heap_buffer(10);
    EXPECT_NE(buf, nullptr);
    delete[] buf;
}

TEST(MakeStackBuffer, SizeIsCorrect) {
    auto buf = make_stack_buffer();
    EXPECT_EQ(buf.size(), 512u);
}
