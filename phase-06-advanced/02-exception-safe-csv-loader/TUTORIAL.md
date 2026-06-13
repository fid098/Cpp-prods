# Tutorial — Exception-Safe CSV Loader

## Step 1 — Custom Exception Hierarchy

```cpp
#include <stdexcept>
#include <string>

struct ParseError : std::runtime_error {
    int         row;
    std::string column;

    ParseError(int row, const std::string& col, const std::string& msg)
        : std::runtime_error{"ParseError row " + std::to_string(row)
                             + " col '" + col + "': " + msg},
          row{row}, column{col}
    {}
};

struct FileError : std::runtime_error {
    using std::runtime_error::runtime_error;  // inherit constructor
};
```

Inheriting from `std::runtime_error` means the caller can catch `std::exception` to handle all errors generically, or `ParseError` specifically for parse failures.

> **Docs:** https://en.cppreference.com/w/cpp/error/exception

---

## Step 2 — RAII File Handling

```cpp
#include <fstream>
#include <stdexcept>

std::ifstream open_file(const std::string& path) {
    std::ifstream file{path};
    if (!file.is_open())
        throw FileError{"Cannot open file: " + path};
    return file;  // ifstream is moveable
}
```

`std::ifstream` closes the file in its destructor — even if an exception is thrown mid-parse. No manual `file.close()` needed.

> **Docs:** https://en.cppreference.com/w/cpp/io/basic_ifstream

---

## Step 3 — Safe String→Number Conversion

```cpp
double parse_double(const std::string& s, int row, const std::string& col) {
    try {
        return std::stod(s);
    } catch (const std::invalid_argument&) {
        throw ParseError{row, col, "expected numeric, got '" + s + "'"};
    } catch (const std::out_of_range&) {
        throw ParseError{row, col, "value out of range: '" + s + "'"};
    }
}
```

`std::stod` throws `std::invalid_argument` for non-numeric input and `std::out_of_range` for overflow.

> **Docs:** https://en.cppreference.com/w/cpp/string/basic_string/stod

---

## Step 4 — Parsing Logic

```cpp
std::vector<OHLCVBar> load_csv(const std::string& path) {
    auto file = open_file(path);   // RAII — closes on any exception
    std::vector<OHLCVBar> bars;
    std::string line;
    std::getline(file, line);  // skip header

    int row = 1;
    while (std::getline(file, line)) {
        ++row;
        // Split on comma, then parse each field with parse_double
        // If any field throws ParseError, it propagates to the caller
        bars.push_back(parse_row(line, row));
    }
    return bars;
}
```

---

## Step 5 — Caller Side

```cpp
try {
    auto bars = load_csv("prices.csv");
    std::cout << "Loaded " << bars.size() << " bars.\n";
} catch (const ParseError& e) {
    std::cerr << "Parse failure: " << e.what() << "\n";
    std::cerr << "  Row=" << e.row << "  Column=" << e.column << "\n";
} catch (const FileError& e) {
    std::cerr << "File error: " << e.what() << "\n";
} catch (const std::exception& e) {
    std::cerr << "Unexpected error: " << e.what() << "\n";
}
```

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Catching by value (`catch (ParseError e)`) | Always catch by const reference: `catch (const ParseError& e)` |
| Throwing `std::string` or `int` | Always throw objects derived from `std::exception` |
| Using `new` to create exception objects | Throw by value: `throw ParseError{...}` |
| Open file, forget to close on exception | Use `std::ifstream` — RAII closes it automatically |
