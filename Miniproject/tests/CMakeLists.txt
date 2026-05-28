add_executable(unit_tests
    test_symbol_table.cpp
    test_printer.cpp
    test_dsl.cpp
    test_simulator.cpp)
target_link_libraries(unit_tests PRIVATE stochastic doctest::doctest_with_main)

add_executable(parallel_bm parallel_bm.cpp)
target_link_libraries(parallel_bm PRIVATE stochastic Threads::Threads benchmark::benchmark_main)
target_include_directories(parallel_bm PRIVATE ${CMAKE_SOURCE_DIR}/examples)  # for networks.hpp
add_benchmark(parallel_bm)  # macro from benchmark.cmake: runs it + writes JSON timings (+ plot if available)

add_test(NAME unit_tests COMMAND unit_tests)
