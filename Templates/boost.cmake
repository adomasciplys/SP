# Ensures boost is installed: either from system or from repository
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.7z
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_PROGRESS TRUE
        DOWNLOAD_NO_EXTRACT FALSE
        USES_TERMINAL_DOWNLOAD TRUE
        USES_TERMINAL_CONFIGURE ON
        USES_TERMINAL_BUILD ON
        USES_TERMINAL_INSTALL ON
        EXCLUDE_FROM_ALL
        FIND_PACKAGE_ARGS 1.90
)

message(STATUS "Preparing Boost, please wait...")
FetchContent_MakeAvailable(Boost)

if (Boost_FOUND)
  message(STATUS "Found Boost: ${Boost_DIR}")
else (Boost_FOUND)
  message(STATUS "Got Boost: ${Boost_SOURCE_DIR}")
endif (Boost_FOUND)

if (TARGET Boost::headers)
  message(STATUS "    Available target: Boost::headers")
endif ()

if (TARGET Boost::type_index)
  message(STATUS "    Available target: Boost::type_index")
endif ()

if (TARGET Boost::math)
  message(STATUS "    Available target: Boost::math")
endif ()

if (TARGET Boost::beast)
  message(STATUS "    Available target: Boost::beast")
endif ()

if (TARGET Boost::graph)
  message(STATUS "    Available target: Boost::graph")
endif ()
