cmake_minimum_required(VERSION 3.8)
project(future_comes_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################
# set up googletest
execute_process(COMMAND git submodule update --init -- libs/gtest
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# set GTEST_INCLUDE_DIR properly
set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googletest/include
        CACHE PATH "path to gtest includes")
# run cmake for gtest too
add_subdirectory(libs/gtest/googletest)
##########################################################


message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target std_ver)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                LINK_LIBRARIES asan
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-Wpedantic;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-pedantic"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message("!!! MSVC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "/W4"
                )
    else ()
        message(FATAL_ERROR "!!! NO KNOWN COMPILER DETECTED, WARNING FLAGS NOT SETTED !!!!")
    endif ()
endmacro()


add_executable(${PROJECT_NAME}
        src/main.cpp
        src/storages/istorage.cpp
        src/storages/row.cpp
        src/storages/array_storage.cpp
        src/storages/stdmap_storage.cpp
        src/storages/avltree_storage.cpp
        src/io/inputfilestream.cpp
        src/stats/function_stats.cpp
        src/stats/class_stats.cpp
        src/stats/istorage_tester.cpp
        )

set_warning_flags(${PROJECT_NAME} 14)


add_executable(smoke_tests
        tests/alltests_runner.cpp
        tests/smoke_tests.cpp
        src/storages/istorage.cpp
        src/storages/row.cpp
        src/storages/array_storage.cpp
        src/storages/stdmap_storage.cpp
        src/storages/avltree_storage.cpp
        )
set_warning_flags(smoke_tests 14)
target_link_libraries(smoke_tests
        gtest
        ${GTEST_MAIN_LIBRARY}
        )

# set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/smoke_tests)
