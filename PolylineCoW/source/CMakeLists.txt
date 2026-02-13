add_library(point OBJECT point.cpp)
add_library(polyline STATIC polyline.cpp)
target_link_libraries(polyline PUBLIC point)

add_library(polyline2 STATIC polyline2.cpp)
target_link_libraries(polyline2 PUBLIC point)
