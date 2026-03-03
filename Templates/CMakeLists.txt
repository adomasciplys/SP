cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0135 NEW) # DOWNLOAD_EXTRACT_TIMESTAMP
if (POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW) # assume no FindBoost
endif()
project(Templates CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clang-tidy

if (CMAKE_CXX_COMPILER_ID MATCHES GNU OR CMAKE_CXX_COMPILER_ID MATCHES Clang)
    add_compile_options(-Wpedantic -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options(/W4)
endif ()

include(sanitizers.cmake)
include(doctest.cmake)
include(boost.cmake)

include_directories(include)

enable_testing()

add_subdirectory(src)
