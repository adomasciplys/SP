include(FetchContent)

FetchContent_Declare(capy
        GIT_REPOSITORY https://github.com/cppalliance/capy.git
        GIT_TAG develop
        GIT_SHALLOW TRUE)
set(BOOST_CAPY_BUILD_TESTS OFF CACHE BOOL "Build boost::capy tests")
set(BOOST_CAPY_BUILD_EXAMPLES OFF CACHE BOOL "Build boost::capy examples")
set(BOOST_CAPY_BUILD_BENCH OFF CACHE BOOL "Build boost::capy benchmarks")
set(BOOST_CAPY_BUILD_P2300_EXAMPLES OFF CACHE BOOL "Build examples that depend on beman-execution (P2300)")
set(BOOST_CAPY_MRDOCS_BUILD OFF CACHE BOOL "Build the target for MrDocs: see mrdocs.yml")

FetchContent_Declare(corosio
        GIT_REPOSITORY https://github.com/cppalliance/corosio.git
        GIT_TAG develop
        GIT_SHALLOW TRUE)
set(BOOST_COROSIO_BUILD_TESTS OFF CACHE BOOL "Build boost::corosio tests")
set(BOOST_COROSIO_BUILD_PERF OFF CACHE BOOL "Build boost::corosio performance tools")
set(BOOST_COROSIO_BUILD_EXAMPLES OFF CACHE BOOL "Build boost::corosio examples")
set(BOOST_COROSIO_MRDOCS_BUILD OFF CACHE BOOL "Building for MrDocs documentation generation")

FetchContent_MakeAvailable(capy corosio)

if(capy_FOUND) # find_package
    message(STATUS "Found Boost::capy: ${capy_DIR}")
else(capy_FOUND) # FetchContent
    message(STATUS "Fetched Boost::capy: ${capy_SOURCE_DIR}")
endif(capy_FOUND)

if(TARGET Boost::capy)
    message(STATUS "    Available target: Boost::capy")
endif()

if(corosio_FOUND) # find_package
    message(STATUS "Found Boost::corosio: ${corosio_DIR}")
else(corosio_FOUND) # FetchContent
    message(STATUS "Fetched Boost::corosio: ${corosio_SOURCE_DIR}")
endif(corosio_FOUND)

if(TARGET Boost::corosio)
    message(STATUS "    Available target: Boost::corosio")
endif()