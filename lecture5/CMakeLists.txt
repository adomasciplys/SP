cmake_minimum_required(VERSION 3.10)
project(lecture5)

set(CMAKE_CXX_STANDARD 20)

add_executable(base base.cpp)

include(FetchContent)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
)

# Disable benchmark tests and examples
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(benchmark)

target_link_libraries(base PRIVATE benchmark::benchmark)
