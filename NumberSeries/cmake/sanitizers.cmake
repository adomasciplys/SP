# Various sanitizers (runtime checks) for debugging
option(SSP "Stack Smashing Protector (GCC/Clang/ApplClang on Unix)" OFF) # Available on Windows too
option(UBSAN "Undefined Behavior Sanitizer (GCC/Clang/AppleClang on Unix)" OFF)
option(ASAN "Address Sanitizer (GCC/Clang/AppleClang on Unix, MSVC on Windows)" OFF)
option(LSAN "Leak Sanitizer (GCC/Clang/AppleClang on Unix, MSVC on Windows)" OFF)
option(TSAN "Thread Sanitizer (GCC/Clang/AppleClang on Unix)" OFF)
option(RTC_C "Runtime Checks for Conversions (MSVC on Windows)" OFF)
option(RTC_S "Runtime Checks for Stack (MSVC on Windows)" OFF)
option(RTC_U "Runtime Checks for Uninitialized (MSVC on Windows)" OFF)

if (SSP)
    add_compile_options(-fstack-protector)
    add_link_options(-fstack-protector)
    message(STATUS "Enable Stack Smashing Protector")
endif(SSP)

if (UBSAN OR ASAN OR LSAN OR TSAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    else()
        add_compile_options(-fno-omit-frame-pointer)
        add_link_options(-fno-omit-frame-pointer)
    endif()
endif()

if (UBSAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/fsanitize=undefined)
        message(STATUS "Please see if MSVC supports undefined behavior sanitizer: https://learn.microsoft.com/en-us/cpp/sanitizers")
    else()
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
    endif()
    message(STATUS "Enabled Undefined Behavior Sanitizer")
endif(UBSAN)

if (ASAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/fsanitize=address /Zi)
    else()
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
    message(STATUS "Enabled Address Sanitizer")
endif(ASAN)

if (LSAN)
    if (ASAN)
        message(WARNING "Address Sanitizer may disable Leak Sanitizer. Use one of them at a time.")
    endif (ASAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # MSVC does not have Leak Sanitizer so we use C Run Time (CRT) leak checker
        add_compile_definitions(_CRTDBG_MAP_ALLOC)
        if(NOT EXISTS ${CMAKE_BINARY_DIR}/debug_new.h)
            file(WRITE ${CMAKE_BINARY_DIR}/debug_new.h
                "#include <crtdbg.h>\n"
                "#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)\n"
            )
        endif ()
        add_compile_options(/FI${CMAKE_BINARY_DIR}/debug_new.h)
        if(NOT EXISTS ${CMAKE_BINARY_DIR}/leak_check.cpp)
            file(WRITE ${CMAKE_BINARY_DIR}/leak_check.cpp
                "#include <crtdbg.h>\n"
                "#include <cstdlib>\n"
                "#include <cstdio>\n"
                "namespace {\n"
                "void check_leaks() {\n"
                "    if (_CrtDumpMemoryLeaks()) // check if there are leaks\n"
                "        std::exit(EXIT_FAILURE);  // Exit with error code\n"
                "    fprintf(stdout, \"No memory leaks detected.\\n\");\n"
                "}\n"
                "struct LeakChecker {\n"
                "    LeakChecker() {\n"
                "        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);\n"
                "        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);\n"
                "        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);\n"
                "        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);\n"
                "        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);\n"
                "        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);\n"
                "        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);\n"
                #"        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);\n"
                "        std::atexit(check_leaks);\n"
                "    }\n"
                "} leak_checker_instance; }\n"
            )
        endif ()
        add_library(leak_check OBJECT ${CMAKE_BINARY_DIR}/leak_check.cpp)
        set(_LEAK_CHECK_OVERRIDE_ACTIVE FALSE CACHE INTERNAL "")
        function(add_executable target) # override add_executable to link leak_check hook
            if(_LEAK_CHECK_OVERRIDE_ACTIVE) # prevent recursion (triggerred by vcpkg.cmake)
                _add_executable(${target} ${ARGN})
                return()
            endif (_LEAK_CHECK_OVERRIDE_ACTIVE)
            set(_LEAK_CHECK_OVERRIDE_ACTIVE TRUE CACHE INTERNAL "")
            _add_executable(${target} ${ARGN})
            set(_LEAK_CHECK_OVERRIDE_ACTIVE FALSE CACHE INTERNAL "")
            if (TARGET ${target})
                get_target_property(is_imported ${target} IMPORTED)
                get_target_property(aliased ${target} ALIASED_TARGET)
                get_target_property(type ${target} TYPE)
                if(NOT is_imported AND NOT aliased AND type STREQUAL "EXECUTABLE")
                    target_link_libraries(${target} PRIVATE leak_check)
                endif()
            endif ()
        endfunction()
    else()
        add_compile_options(-fsanitize=leak)
        add_link_options(-fsanitize=leak)
    endif()
    message(STATUS "Enabled Leak Sanitizer")
endif (LSAN)

if (TSAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/fsanitize=thread)
        message(STATUS "Please see if MSVC supports thread sanitizer: https://learn.microsoft.com/en-us/cpp/sanitizers")
    else()
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
    message(STATUS "Enabled Thread Sanitizer")
endif(TSAN)

if (RTC_C)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/RTCc)
        add_compile_definitions(_ALLOW_RTCc_IN_STL)
        message(STATUS "Enabled Runtime Check Conversions")
    else()
        message(WARNING "Runtime Check Conversions is not enabled for ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif(RTC_C)

if (RTC_S)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/RTCs)
        message(STATUS "Enabled Runtime Check Stack")
    else()
        message(WARNING "Runtime Check Stack is not enabled for ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif(RTC_S)

if (RTC_U)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/RTCu)
        message(STATUS "Enabled Runtime Check Uninitialized")
    else()
        message(WARNING "Runtime Check Uninitialized is not enabled for ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif(RTC_U)
