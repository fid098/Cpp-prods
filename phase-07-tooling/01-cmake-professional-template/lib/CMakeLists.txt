add_library(quantcore STATIC price_filter.cpp)
target_include_directories(quantcore PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(quantcore PUBLIC cxx_std_23)
target_compile_options(quantcore PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>)
target_precompile_headers(quantcore PRIVATE <cmath> <stdexcept> <string>)

install(TARGETS quantcore
    EXPORT  QuantCoreTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
