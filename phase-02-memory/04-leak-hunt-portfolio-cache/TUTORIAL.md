# Tutorial — Leak Hunt: Portfolio Cache

## Step 1 — The Four Bug Patterns

### Bug 1: Leak (missing delete)

```cpp
void leaky_function() {
    int* data = new int[100];
    // function returns without deleting data → 400 bytes leaked every call
}
```

**Fix:** `delete[] data;` before every `return` path.

---

### Bug 2: Double-Free

```cpp
int* p = new int(42);
delete p;
delete p;   // BUG: undefined behaviour — already freed!
```

**Fix:** Set `p = nullptr` immediately after `delete p;`. Calling `delete nullptr` is safe (it is a no-op).

```cpp
delete p;
p = nullptr;
delete p;  // now safe — deletes nullptr
```

> **Docs:** https://en.cppreference.com/w/cpp/language/delete

---

### Bug 3: Use-After-Free

```cpp
int* p = new int(99);
delete p;
std::cout << *p;  // BUG: reading freed memory — undefined behaviour
```

**Fix:** `p = nullptr;` after `delete p;`. Then any access to `*p` will crash cleanly (null dereference) instead of silently reading garbage.

---

### Bug 4: Dangling Pointer from Local

```cpp
int* dangling_ptr() {
    int local = 42;
    return &local;  // BUG: local is destroyed when function returns!
}
```

**Fix:** Either allocate on the heap (`new`) or return by value, not by pointer.

---

## Step 2 — How to Use Address Sanitizer

Compile with ASan flags:

```bash
# On GCC/Clang:
g++ -fsanitize=address -g -o portfolio_cache src/main.cpp
./portfolio_cache

# On MSVC (Visual Studio 2019+):
# Add /fsanitize=address to compiler flags in CMakeLists.txt
```

ASan prints a detailed error report showing the exact line number of the bug.

In CMakeLists.txt, add:
```cmake
option(SANITIZE "Enable Address Sanitizer" OFF)
if(SANITIZE)
    target_compile_options(portfolio_cache PRIVATE -fsanitize=address -g)
    target_link_options(portfolio_cache PRIVATE -fsanitize=address)
endif()
```

> **Docs:** https://clang.llvm.org/docs/AddressSanitizer.html

---

## Step 3 — Reading the Buggy Code

Open `include/portfolio_cache.hpp`. Every bug is marked with a `// BUG:` comment that describes what is wrong. Your task:

1. Read each `// BUG:` comment
2. Find the root cause (wrong `delete`, missing `delete`, dangling reference)
3. Apply the minimal fix (do not rewrite the logic, just fix the memory issue)
4. Rebuild and re-run — ASan should report zero errors

---

## Step 4 — Run the Tests

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build
cd build && ctest --output-on-failure
```

The tests verify that:
- The cache correctly stores and retrieves positions
- `clear()` frees all memory (test runs under ASan to catch leaks)
- The cache handles `nullptr` inputs without crashing

---

## Common Pitfalls

| Mistake | Fix |
|---------|-----|
| Fixing one bug but introducing another | Change only what the `// BUG:` comment identifies |
| Using `delete` on an array | Use `delete[]` for arrays |
| Not setting ptr to `nullptr` after delete | Always `ptr = nullptr` immediately after `delete ptr` |
