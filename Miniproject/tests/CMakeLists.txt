add_executable(unit_tests
    test_symbol_table.cpp
    test_printer.cpp
    test_dsl.cpp)
target_link_libraries(unit_tests PRIVATE stochastic doctest::doctest_with_main)

add_test(NAME unit_tests COMMAND unit_tests)
