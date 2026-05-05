cmake_minimum_required(VERSION 4.1)
project(Miniproject)

set(CMAKE_CXX_STANDARD 20)

# Library code: types, operators, vessel.
add_library(stochastic STATIC
    source/reactant_list.cpp
    source/partial_reaction.cpp
    source/reaction.cpp
    source/vessel.cpp
    source/printer.cpp)
target_include_directories(stochastic PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Example programs.
add_executable(seihr examples/seihr.cpp)
target_link_libraries(seihr PRIVATE stochastic)

add_executable(circadian examples/circadian.cpp)
target_link_libraries(circadian PRIVATE stochastic)
