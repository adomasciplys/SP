add_executable(sum sum.cpp)
target_link_libraries(sum PRIVATE doctest::doctest_with_main)

add_test(NAME sum COMMAND sum_test)