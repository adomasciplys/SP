add_executable(polyline_test polyline_test.cpp)
target_link_libraries(polyline_test PRIVATE polyline)

add_executable(polyline2_test polyline2_test.cpp)
target_link_libraries(polyline2_test PRIVATE polyline2)

enable_testing()
add_test(NAME polyline_test COMMAND polyline_test)
add_test(NAME polyline2_test COMMAND polyline2_test)
