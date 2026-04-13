add_executable(fibonacci_test fibonacci_test.cpp)
target_link_libraries(fibonacci_test PRIVATE fibonacci doctest::doctest_with_main)
add_test(NAME fibonacci_test COMMAND fibonacci_test)

add_executable(collatz_test collatz_test.cpp)
target_link_libraries(collatz_test PRIVATE collatz doctest::doctest_with_main)
add_test(NAME collatz_test COMMAND collatz_test)

add_executable(generator_test generator_test.cpp)
target_link_libraries(generator_test PRIVATE generator doctest::doctest_with_main)
add_test(NAME generator_test COMMAND generator_test)

add_executable(coroutines_test coroutines_test.cpp)
target_link_libraries(coroutines_test PRIVATE coroutines doctest::doctest_with_main)
add_test(NAME coroutines_test COMMAND coroutines_test)

add_executable(corosio_test corosio_test.cpp)
target_link_libraries(corosio_test PRIVATE Boost::corosio doctest::doctest_with_main)
add_test(NAME corosio_test COMMAND corosio_test)
