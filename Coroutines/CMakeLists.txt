cmake_minimum_required(VERSION 3.20)
project(Coroutines)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/sanitizers.cmake)
include(cmake/libcoro.cmake)
include(cmake/corosio.cmake)
include(cmake/doctest.cmake)
if (CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    include(cmake/benchmark.cmake)
else()
    message(WARNING "Benchmarks are disabled in Debug build (use Release instead)")
endif()

include_directories(include)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)