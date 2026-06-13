# 01 — CMake Professional Template

## What You'll Build

A **production-grade CMake project** for a small quant library (`libquantcore`) and a CLI tool that consumes it. The library exposes a `PriceFilter` (exponential moving average) and a `Spread` calculator. The project demonstrates every CMake feature a professional C++ developer encounters.

## Learning Objectives

| CMake Feature | Purpose |
|---------------|---------|
| `INTERFACE` / `PRIVATE` / `PUBLIC` | Propagating include dirs and compile options |
| `target_compile_features` | Requiring C++23 without setting globals |
| `install()` + `export()` | Making the library `find_package`-able |
| `FetchContent` | Vendoring GoogleTest |
| Precompiled headers | Speeding up STL-heavy code |
| `cmake --preset` | Reproducible Debug/Release builds |
| `CPack` | Generating `.zip` / `.tar.gz` distributions |

## Project Structure

```
01-cmake-professional-template/
├── README.md
├── TUTORIAL.md
├── CMakeLists.txt              ← root
├── CMakePresets.json
├── cmake/
│   └── QuantCoreConfig.cmake.in
├── include/
│   └── quantcore/
│       ├── price_filter.hpp
│       └── spread.hpp
├── src/
│   ├── CMakeLists.txt
│   └── main.cpp
├── lib/
│   ├── CMakeLists.txt
│   └── price_filter.cpp        ← only file that needs a .cpp (non-inline impl)
└── tests/
    ├── CMakeLists.txt
    └── test_quantcore.cpp
```

## Key Resources

| Topic | Link |
|-------|------|
| CMake target model | https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html |
| `target_compile_features` | https://cmake.org/cmake/help/latest/command/target_compile_features.html |
| `FetchContent` | https://cmake.org/cmake/help/latest/module/FetchContent.html |
| `install` | https://cmake.org/cmake/help/latest/command/install.html |
| CMake Presets | https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html |
| Precompiled headers | https://cmake.org/cmake/help/latest/command/target_precompile_headers.html |
| CPack | https://cmake.org/cmake/help/latest/module/CPack.html |

## Build & Run

```bash
# Configure (debug)
cmake --preset debug

# Build
cmake --build build/debug

# Test
ctest --test-dir build/debug -V

# Install to local prefix
cmake --install build/debug --prefix ./install

# Package
cd build/debug && cpack
```
