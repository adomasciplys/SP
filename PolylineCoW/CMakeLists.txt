cmake_minimum_required(VERSION 3.20)
project(PolylineCoW)

option(POLYLINE_TESTS "Unit Tests for Polyline" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # creates compile_commands.json in build folder

include_directories(include)
add_subdirectory(source)

if (POLYLINE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif(POLYLINE_TESTS)
