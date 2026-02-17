# Downloads and compiles Google Benchmark
include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.5    # or "main" for latest
        GIT_SHALLOW TRUE  # download specific revision only (git clone --depth 1)
        GIT_PROGRESS TRUE # show download progress in Ninja
        EXCLUDE_FROM_ALL ON
        FIND_PACKAGE_ARGS 1.9)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Enable testing of the benchmark library.")
set(BENCHMARK_ENABLE_EXCEPTIONS ON CACHE BOOL "Enable the use of exceptions in the benchmark library.")
set(BENCHMARK_ENABLE_LTO OFF CACHE BOOL "Enable link time optimisation of the benchmark library.")
set(BENCHMARK_USE_LIBCXX OFF CACHE BOOL "Build and test using libc++ as the standard library.")
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "Build Release candidates with -Werror.")
set(BENCHMARK_FORCE_WERROR OFF CACHE BOOL "Build Release candidates with -Werror regardless of compiler issues.")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Enable installation of benchmark. (Projects embedding benchmark may want to turn this OFF.)")
set(BENCHMARK_ENABLE_DOXYGEN OFF CACHE BOOL "Build documentation with Doxygen.")
set(BENCHMARK_INSTALL_DOCS OFF CACHE BOOL "Enable installation of documentation.")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Allow the downloading and in-tree building of unmet dependencies")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Enable building the unit tests which depend on gtest")
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "Use bundled GoogleTest. If disabled, the find_package(GTest) will be used.")

FetchContent_MakeAvailable(benchmark)

if (benchmark_FOUND) # find_package
    message(STATUS "Found Google Benchmark: ${benchmark_DIR}")
else (benchmark_FOUND) # FetchContent
    message(STATUS "Fetched Google Benchmark: ${benchmark_SOURCE_DIR}")
endif (benchmark_FOUND)

find_file(BENCHMARK_COMPARE_SCRIPT NAMES compare.py
        PATHS ${benchmark_SOURCE_DIR}/tools ${benchmark_DIR} /usr/share/benchmark)
if (BENCHMARK_COMPARE_SCRIPT)
    find_program(PYTHON_COMMAND NAMES python python3)
    if (PYTHON_COMMAND)
        set(benchmark_cmp ${PYTHON_COMMAND} ${BENCHMARK_COMPARE_SCRIPT})
    endif (PYTHON_COMMAND)
endif (BENCHMARK_COMPARE_SCRIPT)

if (TARGET benchmark::benchmark)
    message(STATUS "    Available target: benchmark::benchmark")
endif ()

if (TARGET benchmark::benchmark_main)
    message(STATUS "    Available target: benchmark::benchmark_main")
endif ()
