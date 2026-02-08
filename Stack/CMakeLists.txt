cmake_minimum_required(VERSION 3.28)
project(Stack)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ASAN "Address Sanitizer" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU OR CMAKE_CXX_COMPILER_ID MATCHES Clang)
    add_compile_options(-Wpedantic -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
    if (ASAN)
        add_compile_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
        add_link_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)
        if (WIN32)
            message(WARNING "GCC/Clang sanitizers unlikely to work on ${CMAKE_SYSTEM_NAME}, try Visual Studio instead")
        endif (WIN32)
    endif (ASAN)
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options(/W4)
    if (ASAN)
        add_compile_options(/fsanitize=address /RTCs /RTCu)
        add_link_options(/fsanitize=address /RTCs /RTCu)
    endif (ASAN)
endif ()

add_library(stack OBJECT stack.cpp)

enable_testing()

add_executable(stack_test stack_test.cpp)
target_link_libraries(stack_test PRIVATE stack)
add_test(NAME stack_test COMMAND stack_test)
