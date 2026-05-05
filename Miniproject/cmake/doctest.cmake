# Downloads and compiles doctest unit-testing framework.
include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(DOCTEST_WITH_TESTS OFF CACHE BOOL "Build doctest's own tests/examples")
set(DOCTEST_WITH_MAIN_IN_STATIC_LIB ON CACHE BOOL "Build a static lib for doctest::doctest_with_main")
set(DOCTEST_NO_INSTALL ON CACHE BOOL "Skip doctest installation")
set(DOCTEST_USE_STD_HEADERS OFF CACHE BOOL "Use std headers in doctest")

# doctest 2.4.11's cmake_minimum_required predates CMake 4's removed policies.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version for sub-projects")

FetchContent_Declare(doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE)
FetchContent_MakeAvailable(doctest)
