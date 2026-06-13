# 01 — Template Statistics Library

> Phase 6 · Advanced | C++23 | A reusable stats library that works for any numeric type

## What You'll Build

A header-only statistics library using **class and function templates** that computes mean, variance, standard deviation, skewness, kurtosis, and correlation — for `float`, `double`, or `long double`.

```cpp
std::vector<double> returns = {0.01, -0.02, 0.03, -0.01, 0.02};

Stats<double> s(returns);
std::cout << s.mean()     << "\n";  // 0.006
std::cout << s.std_dev()  << "\n";  // 0.0187
std::cout << s.skewness() << "\n";  // -0.146
```

## Learning Objectives

- Write function templates (`template<typename T>`)
- Write class templates (`template<typename T> class Stats`)
- Understand template type deduction
- Use `static_assert` with `std::is_floating_point` to constrain template types
- Specialise a template for a specific type

## Key Resources

| Concept | Link |
|---------|------|
| Function templates | https://en.cppreference.com/w/cpp/language/function_template |
| Class templates | https://en.cppreference.com/w/cpp/language/class_template |
| std::is_floating_point | https://en.cppreference.com/w/cpp/types/is_floating_point |
| std::accumulate | https://en.cppreference.com/w/cpp/algorithm/accumulate |
