cmake_minimum_required(VERSION 4.1)
project(Miniproject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library code: types, operators, vessel.
add_library(stochastic STATIC
    source/reactant_list.cpp
    source/partial_reaction.cpp
    source/reaction.cpp
    source/vessel.cpp
    source/printer.cpp
    source/simulator.cpp)

include(cmake/doctest.cmake)
include(cmake/benchmark.cmake)

target_include_directories(stochastic PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Parallel simulation (parallel.hpp / thread-pool.hpp) needs a threading runtime.
find_package(Threads REQUIRED)
target_link_libraries(stochastic PUBLIC Threads::Threads)

# Example programs.
add_executable(figure1 examples/figure1.cpp)
target_link_libraries(figure1 PRIVATE stochastic)

add_executable(seihr examples/seihr.cpp)
target_link_libraries(seihr PRIVATE stochastic)

add_executable(circadian examples/circadian.cpp)
target_link_libraries(circadian PRIVATE stochastic)

# Qt6-based plotting
find_package(Qt6 COMPONENTS Charts Widgets QUIET)
if (Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)

    add_library(stochastic_plot STATIC source/plot.cpp)
    target_link_libraries(stochastic_plot PUBLIC stochastic Qt6::Charts Qt6::Widgets)

    # Plot programs write into <project>/figures.
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/figures)

    add_executable(figure1_plot examples/figure1_plot.cpp)
    target_link_libraries(figure1_plot PRIVATE stochastic_plot)

    add_executable(seihr_plot examples/seihr_plot.cpp)
    target_link_libraries(seihr_plot PRIVATE stochastic_plot)

    add_executable(circadian_plot examples/circadian_plot.cpp)
    target_link_libraries(circadian_plot PRIVATE stochastic_plot)

    # Run plot programs from the project root so their relative "figures/"
    # paths land in <project>/figures.
    foreach(target figure1_plot seihr_plot circadian_plot)
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endforeach()
else()
    message(STATUS "Qt6 not found; skipping plot targets (figure1_plot, seihr_plot, circadian_plot).")
endif()

enable_testing()
add_subdirectory(tests)
