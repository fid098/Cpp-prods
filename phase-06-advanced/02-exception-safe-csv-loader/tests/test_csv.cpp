#include <gtest/gtest.h>
#include <fstream>
#include "csv_loader.hpp"

static void write_csv(const std::string& path, const std::string& content) {
    std::ofstream f{path};
    f << content;
}

TEST(CSVLoader, LoadsValidFile) {
    write_csv("valid.csv",
        "date,open,high,low,close,volume\n"
        "2024-01-01,100.0,105.0,99.0,103.0,500000\n"
        "2024-01-02,103.0,107.0,102.0,106.0,600000\n");
    auto bars = load_ohlcv("valid.csv");
    ASSERT_EQ(bars.size(), 2u);
    EXPECT_DOUBLE_EQ(bars[0].close, 103.0);
    std::remove("valid.csv");
}

TEST(CSVLoader, ThrowsFileErrorOnMissingFile) {
    EXPECT_THROW(load_ohlcv("does_not_exist.csv"), FileError);
}

TEST(CSVLoader, ThrowsParseErrorOnBadDouble) {
    write_csv("bad.csv",
        "date,open,high,low,close,volume\n"
        "2024-01-01,100.0,105.0,99.0,N/A,500000\n");
    EXPECT_THROW(load_ohlcv("bad.csv"), ParseError);
    std::remove("bad.csv");
}

TEST(CSVLoader, ThrowsParseErrorOnHighLessThanLow) {
    write_csv("bad2.csv",
        "date,open,high,low,close,volume\n"
        "2024-01-01,100.0,90.0,105.0,98.0,500000\n");  // high=90 < low=105
    EXPECT_THROW(load_ohlcv("bad2.csv"), ParseError);
    std::remove("bad2.csv");
}

TEST(ParseDouble, ValidReturnsValue) {
    EXPECT_DOUBLE_EQ(parse_double("3.14", 1, "x"), 3.14);
}

TEST(ParseDouble, InvalidThrowsParseError) {
    EXPECT_THROW(parse_double("abc", 1, "col"), ParseError);
}

TEST(SplitCsv, SplitsCorrectly) {
    auto fields = split_csv("a,b,c,d");
    ASSERT_EQ(fields.size(), 4u);
    EXPECT_EQ(fields[0], "a");
    EXPECT_EQ(fields[3], "d");
}
