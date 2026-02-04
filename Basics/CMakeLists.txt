cmake_minimum_required(VERSION 3.15)
project(Basics)

# Specifies the C++ standard used in this project:
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Creates `compile_commands.json` in the build folder, useful to debug building process and other tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Creates object files with relocatable symbols
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU OR CMAKE_CXX_COMPILER_ID MATCHES Clang)
    add_compile_options(-Wpedantic -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options(/W4)
endif ()

# Enables the build of tests
enable_testing()

# Simple self-contained executable program:
add_executable(types types.cpp)
add_test(NAME types COMMAND types)

# Add swap to project
add_executable(swap swap_main.cpp swap.cpp)
add_executable(swap_bm swap_bm.cpp swap.cpp)

add_test(NAME swap_bm COMMAND swap_bm)
set_tests_properties(swap_bm swap_bm PROPERTIES RUN_SERIAL ON)

# Tests for measure.hpp library:
add_executable(measure_test measure_test.cpp)
add_test(NAME measure_test COMMAND measure_test 20)

# Implementation of various argument passing functions in a library:
add_library(passing OBJECT passing.cpp)
# Benchmark program for passing library
add_executable(passing_bm passing_bm.cpp)
target_link_libraries(passing_bm PRIVATE passing) # links benchmark with the library
add_test(NAME passing_bm COMMAND passing_bm 5) # a test command `passing_bm 5`
set_tests_properties(measure_test passing_bm PROPERTIES RUN_SERIAL ON) # run independently
