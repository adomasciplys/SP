add_library(fibonacci OBJECT fibonacci.cpp)
target_compile_features(fibonacci PUBLIC cxx_std_23)

add_library(generator OBJECT generator.cpp)
target_compile_features(generator PUBLIC cxx_std_20)

add_library(coroutines OBJECT coroutines.cpp)
target_compile_features(coroutines PUBLIC cxx_std_20)
target_link_libraries(coroutines PUBLIC libcoro)
