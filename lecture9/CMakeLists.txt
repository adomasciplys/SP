cmake_minimum_required(VERSION 3.15)
project(lecture9 CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/sanitizers.cmake)
include(cmake/doctest.cmake)

#find_program(CLANG_TIDY NAMES clang-tidy)
if (CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
endif (CLANG_TIDY)

include_directories(include)

enable_testing()

add_subdirectory(src)