cmake_minimum_required(VERSION 3.25)
project(QuantCore VERSION 1.0.0 LANGUAGES CXX)

# Only set standards via target_compile_features — never globally
include(GNUInstallDirs)
include(CTest)

# ---- Library ----------------------------------------------------------------
add_subdirectory(lib)

# ---- CLI executable ---------------------------------------------------------
add_subdirectory(src)

# ---- Tests ------------------------------------------------------------------
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()

# ---- Install / Export -------------------------------------------------------
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT QuantCoreTargets
    FILE      QuantCoreTargets.cmake
    NAMESPACE QuantCore::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantCore)

configure_file(cmake/QuantCoreConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QuantCoreConfig.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QuantCoreConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuantCore)

# ---- CPack ------------------------------------------------------------------
set(CPACK_PROJECT_NAME    ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
