add_executable(digits_test digits_test.cpp)
target_compile_definitions(digits_test PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
target_link_libraries(digits_test PRIVATE doctest::doctest)
add_test(NAME digits_test COMMAND digits_test)

add_executable(digits_bm digits_bm.cpp)
target_link_libraries(digits_bm PRIVATE benchmark::benchmark_main)

add_library(stringsort OBJECT stringsort.cpp)

add_executable(mtqueue mtqueue.cpp)
target_link_libraries(mtqueue PRIVATE Threads::Threads)
add_test(NAME mtqueue COMMAND mtqueue)

add_executable(futures futures.cpp)
target_link_libraries(futures PRIVATE Threads::Threads)

add_executable(fischer fischer.cpp)
target_compile_features(fischer PRIVATE cxx_std_23)
target_link_libraries(fischer PRIVATE Threads::Threads)
