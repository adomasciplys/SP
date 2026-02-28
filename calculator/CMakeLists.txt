cmake_minimum_required(VERSION 3.10...3.28)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(calculator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

include(sanitizers.cmake)
include(doctest.cmake)

add_executable(calculator_test
    calculator_test.cpp
    const.cpp
    unary.cpp
    binary.cpp
    var.cpp
    assign.cpp
    expr.cpp
    symbol_table.cpp
    printer.cpp
)

target_link_libraries(calculator_test PRIVATE doctest::doctest_with_main)

enable_testing()
add_test(NAME calculator_test COMMAND calculator_test)
