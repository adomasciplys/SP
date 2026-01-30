cmake_minimum_required(VERSION 3.15)
project(DevTest)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
# Show all the build commands:
# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)
# Export list of compilation commands for external clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WITH_TESTS "Enable Unit Tests" ON)
option(UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(ASAN "Enable Address Sanitizer" OFF)
option(TSAN "Enable Thread Sanitizer" OFF)

find_package(Threads REQUIRED)
if (NOT MSVC)
   find_package(TBB) # MSVC does not require ThreadBuildingBlocks
endif()
if (TBB_FOUND)
    add_definitions(-DTBB_SUPPRESS_DEPRECATED_MESSAGES)
    message(STATUS "Enabled TBB for parallel execution of algorithms")
endif(TBB_FOUND)

find_package(OpenMP)
if (OPENMP_FOUND)
    add_compile_options(${OpenMP_CXX_FLAGS})
endif(OPENMP_FOUND)

if (MSVC) # Visual Studio compiler
    add_compile_options(/W4) # add more warnings
    # sanitizers: https://devblogs.microsoft.com/cppblog/address-sanitizer-for-msvc-now-generally-available/
    # docs: https://docs.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-160
    if (UBSAN OR ASAN OR TSAN)
        add_compile_options(/Zi) # make debug PDB but disable "edit and continue"/ZI
        # add_link_options(/INCREMENTAL:NO)
        # find cl directory with all the runtime libraries:
        get_filename_component(MSVC_BIN ${CMAKE_CXX_COMPILER} DIRECTORY)
        message(STATUS "MSVC_BIN=${MSVC_BIN}")
        string(REPLACE "/bin/" "/lib/" MSVC_LIB ${MSVC_BIN})
        message(STATUS "MSVC_LIB=${MSVC_LIB}")
    endif()
    if (UBSAN)
        # add_compile_options(/fsanitize=undefined) # not supported
    endif(UBSAN)
    if (ASAN)
        add_compile_options(/fsanitize=address)
        # add_link_options(/INFERASANLIBS:NO)
        # copy the DLLs as the binaries need them:
        add_custom_target(asan_libs ALL DEPENDS clang_rt.asan_dynamic-x86_64.dll clang_rt.asan_dbg_dynamic-x86_64.dll)
        add_custom_command(OUTPUT clang_rt.asan_dynamic-x86_64.dll
                COMMAND ${CMAKE_COMMAND} -E copy ${MSVC_BIN}/clang_rt.asan_dynamic-x86_64.dll ${CMAKE_CURRENT_BINARY_DIR}/)
        add_custom_command(OUTPUT clang_rt.asan_dbg_dynamic-x86_64.dll
                COMMAND ${CMAKE_COMMAND} -E copy ${MSVC_BIN}/clang_rt.asan_dbg_dynamic-x86_64.dll ${CMAKE_CURRENT_BINARY_DIR}/)
        message(STATUS "Enabled Address Sanitizer")
    endif(ASAN)
    if (TSAN)
        add_compile_options(/fsanitize=thread)
        message(STATUS "Enabled Thread Sanitizer")
    endif(TSAN)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # GNU Compiler Collection
    add_compile_options(-Wall -Wextra)
    if (UBSAN)
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
        message(STATUS "Enabled Undefined Behavior Sanitizer")
    endif(UBSAN)
    if (ASAN)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
        message(STATUS "Enabled Address Sanitizer")
    endif(ASAN)
    if (TSAN)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
        message(STATUS "Enabled Thread Sanitizer")
    endif(TSAN)
    if (UBSAN OR ASAN OR TSAN)
        add_compile_options(-fno-omit-frame-pointer)
        add_link_options(-fno-omit-frame-pointer)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") # Clang/LLVM compiler
    if (UBSAN)
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
        message(STATUS "Enabled Undefined Behavior Sanitizer")
    endif(UBSAN)
    if (ASAN)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
        message(STATUS "Enabled Address Sanitizer")
    endif(ASAN)
    if (TSAN)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
        message(STATUS "Enabled Thread Sanitizer")
    endif(TSAN)
    if (UBSAN OR ASAN OR TSAN)
        add_compile_options(-fno-omit-frame-pointer)
        add_link_options(-fno-omit-frame-pointer)
    endif()
    if (WIN32)
        # message(STATUS "Using stdc++")
        # add_link_options(-stdlib=libstdc++)
        # Workaround for "Mingw-w64 runtime failure: 32 bit pseudo relocation at..."
        # https://github.com/msys2/MINGW-packages/issues/10547
        add_compile_options(-femulated-tls)
    endif()
endif()

if (MINGW)
  message(STATUS "Detected MINGW")
  link_libraries(stdc++exp) # C++23 is still experimental on MSYS
endif(MINGW)

if (WITH_TESTS)
    enable_testing()
    message(STATUS "Enabled Unit Tests")
endif (WITH_TESTS)

add_executable(hello hello.cpp)
add_executable(hello-cpp23 hello-cpp23.cpp)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Threads::Threads)
if (TBB_FOUND)
    target_link_libraries(main PRIVATE TBB::tbb)
endif(TBB_FOUND)
if (MSVC AND ASAN)
    add_dependencies(main asan_libs)
endif()
if (WITH_TESTS)
    add_test(NAME hello COMMAND hello)
    add_test(NAME hello-cpp23 COMMAND hello-cpp23)
    add_test(NAME main COMMAND main)
endif (WITH_TESTS)
