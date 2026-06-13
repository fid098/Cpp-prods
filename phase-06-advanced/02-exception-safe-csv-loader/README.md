# 02 — Exception-Safe CSV Loader

> Phase 6 · Advanced | C++23 | Load financial CSV data with strong validation and clear failures

## What You'll Build

A robust CSV loader for OHLCV (open, high, low, close, volume) data that throws descriptive exceptions on any formatting or validation error, uses RAII to manage file handles, and provides a clean error-recovery story for the caller.

```
Loading prices.csv...
Row 42: invalid close price "N/A" — ParseError thrown
  → caught: ParseError at row 42, column "close": expected numeric, got "N/A"

Loading clean_prices.csv...
  Loaded 252 OHLCV bars for AAPL.
```

## Learning Objectives

- Define a custom exception hierarchy (`std::runtime_error` subclasses)
- Use `try/catch/throw` correctly
- Understand RAII for file handles (`std::ifstream` closes automatically)
- Use `std::stod` / `std::stoi` with exception handling
- Differentiate: `std::invalid_argument` (parse error) vs `std::out_of_range` (overflow)
- Use `noexcept` on functions that truly cannot throw

## Key Resources

| Concept | Link |
|---------|------|
| Exceptions | https://en.cppreference.com/w/cpp/language/exceptions |
| std::exception | https://en.cppreference.com/w/cpp/error/exception |
| ifstream (RAII) | https://en.cppreference.com/w/cpp/io/basic_ifstream |
| std::stod | https://en.cppreference.com/w/cpp/string/basic_string/stod |
| noexcept | https://en.cppreference.com/w/cpp/language/noexcept |
