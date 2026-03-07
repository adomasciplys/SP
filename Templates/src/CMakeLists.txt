add_executable(kindof_test kindof_test.cpp)
target_link_libraries(kindof_test PRIVATE doctest::doctest_with_main)
add_test(NAME kindof_test COMMAND kindof_test)

add_executable(type_name_test type_name_test.cpp)
target_link_libraries(type_name_test PRIVATE doctest::doctest_with_main)
add_test(NAME type_name_test COMMAND type_name_test)

add_executable(deduction deduction.cpp)
target_link_libraries(deduction PRIVATE doctest::doctest_with_main)
add_test(NAME deduction COMMAND deduction)

add_library(random OBJECT random.cpp)
add_library(minmax OBJECT minmax.cpp)

add_executable(minmax_test minmax_test.cpp)
target_link_libraries(minmax_test PRIVATE minmax random doctest::doctest_with_main)
add_test(NAME minmax_test COMMAND minmax_test)

add_executable(minmax_bm minmax_bm.cpp)
target_link_libraries(minmax_bm PRIVATE minmax random)
add_test(NAME minmax_bm COMMAND minmax_bm)

add_executable(fibonacci_test fibonacci_test.cpp)
target_link_libraries(fibonacci_test PRIVATE doctest::doctest_with_main)
add_test(NAME fibonacci_test COMMAND fibonacci_test)

add_executable(power_test power_test.cpp)
target_link_libraries(power_test PRIVATE doctest::doctest_with_main)
add_test(NAME power_test COMMAND power_test)

add_executable(power_bm power_bm.cpp)

add_executable(type_name_boost type_name_boost.cpp)
target_link_libraries(type_name_boost PRIVATE doctest::doctest_with_main)
if(TARGET Boost::type_index)
    target_link_libraries(type_name_boost PRIVATE Boost::type_index)
endif()
add_test(NAME type_name_boost COMMAND type_name_boost)

