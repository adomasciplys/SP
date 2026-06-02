cmake_minimum_required(VERSION 3.15)
project(FunctorDesign)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

include(cmake/doctest.cmake)
enable_testing()
add_subdirectory(tests)
