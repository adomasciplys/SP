cmake_minimum_required(VERSION 4.1)
project(Miniproject)

set(CMAKE_CXX_STANDARD 20)

add_executable(Miniproject main.cpp source/vessel.cpp)

target_include_directories(Miniproject PRIVATE ${CMAKE_SOURCE_DIR}/include)
