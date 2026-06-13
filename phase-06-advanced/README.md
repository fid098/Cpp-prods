# Phase 6 — Advanced

**C++ concepts covered:** class and function templates, template specialisation, SFINAE basics, exceptions, RAII, file I/O with exceptions, `std::thread`, `std::future`, `std::mutex`, `std::atomic`, lock-free design

Phase 6 takes you to the upper tier of the language: generic code that works for any numeric type, robust error handling that cannot be accidentally ignored, and concurrency that scales to multi-core hardware.

## Projects

| # | Project | Portfolio story | Key concepts |
|---|---------|----------------|--------------|
| 01 | [template-statistics-library](01-template-statistics-library/) | Reusable stats for returns and risk | function/class templates |
| 02 | [exception-safe-csv-loader](02-exception-safe-csv-loader/) | Financial CSV loader with strong validation | exceptions, RAII, file I/O |
| 03 | [multithreaded-monte-carlo](03-multithreaded-monte-carlo/) | Parallel Monte Carlo option pricer | threads, futures, mutexes |
| 04 | [atomic-market-data-cache](04-atomic-market-data-cache/) | Lock-free quote cache | atomics, locks, concurrency design |

## Phase Resources

- https://en.cppreference.com/w/cpp/language/templates
- https://en.cppreference.com/w/cpp/language/exceptions
- https://en.cppreference.com/w/cpp/thread
- https://en.cppreference.com/w/cpp/atomic/atomic
- https://en.cppreference.com/w/cpp/io/basic_fstream
