cmake_minimum_required(VERSION 3.14)
project(longest_valid_parentheses LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# Header-only solution consumed by both targets.
add_library(solution INTERFACE)
target_include_directories(solution INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Command-line driver.
add_executable(lvp src/main.cpp)
target_link_libraries(lvp PRIVATE solution)

# Test suite.
enable_testing()
add_executable(tests tests/test_solution.cpp)
target_link_libraries(tests PRIVATE solution)
add_test(NAME unit_tests COMMAND tests)
