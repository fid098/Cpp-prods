#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

// ── Exception hierarchy ───────────────────────────────────────────────────────

struct FileError : std::runtime_error {
    explicit FileError(const std::string& msg) : std::runtime_error{msg} {}
};

struct ParseError : std::runtime_error {
    int         row;
    std::string column;
    ParseError(int row_, const std::string& col, const std::string& detail)
        : std::runtime_error{"ParseError row=" + std::to_string(row_)
                             + " col='" + col + "': " + detail},
          row{row_}, column{col}
    {}
};

// ── Data types ────────────────────────────────────────────────────────────────

struct OHLCVBar {
    std::string date;
    double open{0.0}, high{0.0}, low{0.0}, close{0.0};
    long long volume{0};
};

// ── Helpers ───────────────────────────────────────────────────────────────────

inline double parse_double(const std::string& s, int row, const std::string& col) {
    try { return std::stod(s); }
    catch (const std::invalid_argument&) {
        throw ParseError{row, col, "expected numeric, got '" + s + "'"};
    }
    catch (const std::out_of_range&) {
        throw ParseError{row, col, "value out of range: '" + s + "'"};
    }
}

inline long long parse_int(const std::string& s, int row, const std::string& col) {
    try { return std::stoll(s); }
    catch (const std::invalid_argument&) {
        throw ParseError{row, col, "expected integer, got '" + s + "'"};
    }
    catch (const std::out_of_range&) {
        throw ParseError{row, col, "integer out of range: '" + s + "'"};
    }
}

// Splits a CSV line into fields (handles commas but not quoted commas)
inline std::vector<std::string> split_csv(const std::string& line) {
    std::vector<std::string> fields;
    std::istringstream ss{line};
    std::string field;
    while (std::getline(ss, field, ',')) fields.push_back(field);
    return fields;
}

// ── Main loader ───────────────────────────────────────────────────────────────

// Expects CSV with header: date,open,high,low,close,volume
// Throws FileError if the file cannot be opened.
// Throws ParseError on any invalid row.
inline std::vector<OHLCVBar> load_ohlcv(const std::string& path) {
    std::ifstream file{path};
    if (!file.is_open())
        throw FileError{"Cannot open file: " + path};

    std::vector<OHLCVBar> bars;
    std::string line;
    std::getline(file, line);  // skip header

    int row = 1;
    while (std::getline(file, line)) {
        ++row;
        if (line.empty()) continue;

        auto fields = split_csv(line);
        if (fields.size() < 6)
            throw ParseError{row, "row", "expected 6 columns, got " + std::to_string(fields.size())};

        OHLCVBar bar;
        bar.date   = fields[0];
        bar.open   = parse_double(fields[1], row, "open");
        bar.high   = parse_double(fields[2], row, "high");
        bar.low    = parse_double(fields[3], row, "low");
        bar.close  = parse_double(fields[4], row, "close");
        bar.volume = parse_int(fields[5], row, "volume");

        if (bar.high < bar.low)
            throw ParseError{row, "high/low", "high < low"};

        bars.push_back(bar);
    }
    return bars;
}
