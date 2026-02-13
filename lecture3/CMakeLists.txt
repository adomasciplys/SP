cmake_minimum_required(VERSION 4.1)
project(lecture3)

set(CMAKE_CXX_STANDARD 14)

add_executable(spy spy.cpp spy.hpp)
add_executable(exc3_2 exc3.2.cpp spy.hpp)

add_executable(exc3_3 exc3.3.cpp spy.hpp)
target_compile_features(exc3_3 PRIVATE cxx_std_17)
target_compile_options(exc3_3 PRIVATE -fno-elide-constructors)

add_executable(exc3_4 exc3.4.cpp spy.hpp)
target_compile_features(exc3_4 PRIVATE cxx_std_17)
target_compile_options(exc3_4 PRIVATE -fno-elide-constructors)

add_executable(lambda lambda.cpp)