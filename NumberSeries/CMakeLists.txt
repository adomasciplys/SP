cmake_minimum_required(VERSION 3.15)
project(NumberSeries)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

include(cmake/sanitizers.cmake)
include(cmake/doctest.cmake)
include(cmake/benchmark.cmake)

option(NUMBER_SERIES_TESTS "Unit Tests for NumberSeries" ${PROJECT_IS_TOP_LEVEL})
if (PROJECT_IS_TOP_LEVEL AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    option(NUMBER_SERIES_BM "Benchmarks for NumberSeries" ON)
else ()
    option(NUMBER_SERIES_BM "Benchmarks for NumberSeries" OFF)
endif ()

include_directories(include)

add_subdirectory(source)

if (NUMBER_SERIES_TESTS)
    message(STATUS "Enabled NumberSeries Tests")
    enable_testing()
    add_subdirectory(tests)
else (NUMBER_SERIES_TESTS)
    message(STATUS "Disabled NumberSeries Tests")
endif (NUMBER_SERIES_TESTS)
