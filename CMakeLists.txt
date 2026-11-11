cmake_minimum_required(VERSION 3.15)
project(ExpenseTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Conan integration
# This assumes you've run 'conan install . --build=missing -s compiler.libcxx=libstdc++11' (or similar)
# and the conan_toolchain.cmake and a find/config file for each dependency are generated in the build directory.
# The CMAKE_PREFIX_PATH will be set by conan_toolchain.cmake to find dependencies.
# If CMAKE_TOOLCHAIN_FILE is not set when running cmake, conan_toolchain.cmake needs to be included manually.
# However, best practice with modern Conan is to use a CMake preset or pass CMAKE_TOOLCHAIN_FILE.

# If you are running cmake from a build directory (e.g., build/)
# and conan generated files are there:
# list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}) # For CMakeDeps
# include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake) # For toolchain

# A more common approach when not using CMake presets:
# Check if conan_toolchain.cmake exists in the build directory.
# This path might need adjustment based on your build directory structure.
# Typically, Conan generates these files in the CMake build directory.
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
else()
    message(WARNING "conan_toolchain.cmake not found in CMAKE_BINARY_DIR. Dependencies might not be found if not installed globally or handled by another method.")
endif()


# Include directories
include_directories(include)

# Add executable
add_executable(expense_tracker
    src/main.cpp
    src/Expense.cpp
    src/ExpenseManager.cpp
)

# Optional source-header mode for an isolated verification/development runtime.
# Default Conan package discovery remains unchanged.
set(EXPENSE_HEADER_ROOT "" CACHE PATH "Optional prefix containing actual dependency headers")
if(EXPENSE_HEADER_ROOT)
    find_path(EXPENSE_FMT_INCLUDE fmt/core.h PATHS "${EXPENSE_HEADER_ROOT}/include" NO_DEFAULT_PATH REQUIRED)
    find_path(EXPENSE_DATE_INCLUDE date/date.h PATHS "${EXPENSE_HEADER_ROOT}/include" NO_DEFAULT_PATH REQUIRED)
    find_path(EXPENSE_CSV_INCLUDE csv.h PATHS "${EXPENSE_HEADER_ROOT}/include" "${EXPENSE_HEADER_ROOT}/include/libfccp" NO_DEFAULT_PATH REQUIRED)
    find_package(Threads REQUIRED)
    target_include_directories(expense_tracker PRIVATE ${EXPENSE_FMT_INCLUDE} ${EXPENSE_DATE_INCLUDE} ${EXPENSE_CSV_INCLUDE})
    target_compile_definitions(expense_tracker PRIVATE FMT_HEADER_ONLY)
    target_link_libraries(expense_tracker PRIVATE Threads::Threads)
else()
    find_package(fmt REQUIRED)
    find_package(date REQUIRED)
    find_package(fast-cpp-csv-parser REQUIRED)
    target_link_libraries(expense_tracker PRIVATE fmt::fmt date::date fast-cpp-csv-parser::fast-cpp-csv-parser)
endif()

# Installation (Optional, but good practice)
# install(TARGETS expense_tracker DESTINATION bin)

# Enable testing with CTest (Optional, if you add tests later)
# enable_testing()
# add_test(NAME MyTest COMMAND expense_tracker --some-test-arg)
