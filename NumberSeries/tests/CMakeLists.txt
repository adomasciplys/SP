add_executable(main main.cpp)
target_link_libraries(main PRIVATE number_series)
add_test(NAME main COMMAND main)

add_executable(number_series_test number_series_test.cpp)
target_link_libraries(number_series_test PRIVATE number_series doctest::doctest_with_main)
add_test(NAME number_series_test COMMAND number_series_test)

if (NUMBER_SERIES_BM)
    message(STATUS "Enabled NumberSeries Benchmarks")

    add_executable(sort_bm sort_bm.cpp)
    target_link_libraries(sort_bm PRIVATE benchmark::benchmark_main)
    add_test(NAME sort_bm COMMAND sort_bm --benchmark_out_format=json --benchmark_out=sort_bm.json)

    add_executable(number_series_bm number_series_bm.cpp)
    target_link_libraries(number_series_bm PRIVATE number_series benchmark::benchmark_main)
    add_test(NAME number_series_bm COMMAND number_series_bm --benchmark_out_format=json --benchmark_out=number_series_bm.json)

    set_tests_properties(sort_bm number_series_bm PROPERTIES RUN_SERIAL TRUE)

    if (PYTHON_COMMAND)
        add_test(NAME sort_plot
            COMMAND ${PYTHON_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/plot.py --logx --logy sort_bm.json)
        set_tests_properties(sort_plot PROPERTIES DEPENDS sort_bm)

        add_test(NAME number_series_plot
            COMMAND ${PYTHON_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/plot.py --logx --logy number_series_bm.json)
        set_tests_properties(number_series_plot PROPERTIES DEPENDS number_series_bm)
    endif(PYTHON_COMMAND)
else (NUMBER_SERIES_BM)
    message(STATUS "Disabled NumberSeries Benchmarks")
endif (NUMBER_SERIES_BM)