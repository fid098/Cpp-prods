#include <gtest/gtest.h>
#include "stats.hpp"

TEST(Stats, MeanKnownValue) {
    Stats<double> s{{1.0, 2.0, 3.0, 4.0, 5.0}};
    EXPECT_DOUBLE_EQ(s.mean(), 3.0);
}

TEST(Stats, VarianceKnownValue) {
    // sample var of {1,2,3,4,5} = 2.5
    Stats<double> s{{1.0, 2.0, 3.0, 4.0, 5.0}};
    EXPECT_DOUBLE_EQ(s.variance(), 2.5);
}

TEST(Stats, StdDevIsSquareRootOfVariance) {
    Stats<double> s{{1.0, 2.0, 3.0, 4.0, 5.0}};
    EXPECT_NEAR(s.std_dev(), std::sqrt(2.5), 1e-10);
}

TEST(Stats, SymmetricDistributionSkewnessNearZero) {
    // {1,2,3,4,5} is symmetric around 3
    Stats<double> s{{1.0, 2.0, 3.0, 4.0, 5.0}};
    EXPECT_NEAR(s.skewness(), 0.0, 1e-10);
}

TEST(Stats, MinMax) {
    Stats<double> s{{5.0, 3.0, 1.0, 4.0, 2.0}};
    EXPECT_DOUBLE_EQ(s.min(), 1.0);
    EXPECT_DOUBLE_EQ(s.max(), 5.0);
}

TEST(Stats, FloatType) {
    Stats<float> s{{1.0f, 2.0f, 3.0f}};
    EXPECT_NEAR(s.mean(), 2.0f, 1e-5f);
}

TEST(Correlation, PerfectPositive) {
    std::vector<double> x = {1,2,3,4,5};
    std::vector<double> y = {2,4,6,8,10};
    EXPECT_NEAR(correlation(x, y), 1.0, 1e-10);
}

TEST(Correlation, PerfectNegative) {
    std::vector<double> x = {1,2,3,4,5};
    std::vector<double> y = {10,8,6,4,2};
    EXPECT_NEAR(correlation(x, y), -1.0, 1e-10);
}

TEST(Stats, EmptyThrows) {
    EXPECT_THROW(Stats<double>({}), std::invalid_argument);
}
