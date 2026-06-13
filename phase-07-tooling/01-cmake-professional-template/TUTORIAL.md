# Tutorial — CMake Professional Template

## Step 1 — The Target Model

The cardinal rule of modern CMake: **never use directory-level commands like `include_directories()` or `add_compile_options()`**. Always use target-scoped commands:

```cmake
target_include_directories(mylib PUBLIC include/)
target_compile_options(mylib PRIVATE -Wall -Wextra)
target_compile_features(mylib PUBLIC cxx_std_23)
```

The three visibility keywords:
| Keyword | What it means |
|---------|---------------|
| `PRIVATE` | Only this target gets it |
| `PUBLIC` | This target AND any target that links to it |
| `INTERFACE` | Only targets that link to this target (not this target itself) — used for header-only libs |

> **Docs:** https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html

---

## Step 2 — Library vs Executable

```cmake
# lib/CMakeLists.txt
add_library(quantcore STATIC price_filter.cpp)
target_include_directories(quantcore PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(quantcore PUBLIC cxx_std_23)

# src/CMakeLists.txt
add_executable(quant_cli main.cpp)
target_link_libraries(quant_cli PRIVATE quantcore)
# No need to repeat include dirs — they propagate via PUBLIC
```

> **Docs:** https://cmake.org/cmake/help/latest/command/add_library.html

---

## Step 3 — Precompiled Headers

STL headers like `<vector>`, `<string>`, `<map>` are compiled hundreds of times in large projects. Precompiled headers (PCH) compile them once:

```cmake
target_precompile_headers(quantcore PRIVATE
    <vector>
    <string>
    <cmath>
    <algorithm>
)
```

> **Docs:** https://cmake.org/cmake/help/latest/command/target_precompile_headers.html

---

## Step 4 — CMake Presets

`CMakePresets.json` defines named configurations so anyone can reproduce your build:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "debug",
      "binaryDir": "${sourceDir}/build/debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_CXX_FLAGS": "-g -fsanitize=address,undefined"
      }
    },
    {
      "name": "release",
      "binaryDir": "${sourceDir}/build/release",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Release" }
    }
  ]
}
```

```bash
cmake --preset debug
cmake --build build/debug
```

> **Docs:** https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html

---

## Step 5 — Install and Export

Make your library `find_package`-able by other projects:

```cmake
include(GNUInstallDirs)
install(TARGETS quantcore
        EXPORT  QuantCoreTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT QuantCoreTargets
        FILE       QuantCoreTargets.cmake
        NAMESPACE  QuantCore::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantCore)
```

Now another project can do:
```cmake
find_package(QuantCore REQUIRED)
target_link_libraries(myapp PRIVATE QuantCore::quantcore)
```

> **Docs:** https://cmake.org/cmake/help/latest/command/install.html

---

## Step 6 — Separate Test CMakeLists

Keep tests in their own `CMakeLists.txt` and add it with:

```cmake
# root CMakeLists.txt
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(tests)
endif()
```

This lets CI skip tests with `-DBUILD_TESTING=OFF` for release packaging.

---

## Step 7 — CPack for Distribution

```cmake
# At the end of root CMakeLists.txt
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
```

```bash
cd build/release && cpack      # generates a .zip and .tar.gz
```

> **Docs:** https://cmake.org/cmake/help/latest/module/CPack.html

---

## Common CMake Mistakes

| Mistake | Fix |
|---------|-----|
| `include_directories(...)` | `target_include_directories(... PUBLIC ...)` |
| `set(CMAKE_CXX_STANDARD 23)` globally | `target_compile_features(... PUBLIC cxx_std_23)` |
| Hardcoding `/usr/include` | Use `find_package` + imported targets |
| Not setting `RPATH` | Use `set(CMAKE_INSTALL_RPATH ...)` |
| Globbing source files with `file(GLOB ...)` | List files explicitly — CMake won't re-run on new files |
