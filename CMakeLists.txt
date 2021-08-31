cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(CherryBlazerUtil)

project(cherry_blazer LANGUAGES CXX)

if(CMAKE_BUILD_TYPE)
    message(
        FATAL_ERROR
            "Don't set CMAKE_BUILD_TYPE for this project. \
Instead, set more granular options specific to cherry_blazer. To get them:
    cmake -S . -B build -LH | grep -B2 CHERRY_BLAZER")
endif()

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(FATAL_ERROR "Direct inclusion of cherry_blazer as a subproject is not supported.")
endif()

if(NOT (UNIX AND NOT APPLE))
    message(FATAL_ERROR "cherry_blazer only supports Linux.")
endif()

option(CHERRY_BLAZER_CI "Set ON if building in CI" OFF)

# Compilation database for code completion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set of flags built up piecewise to be used to build sources of every target defined by this
# project.
add_library(cherry_blazer_flags INTERFACE)

target_compile_options(cherry_blazer_flags INTERFACE -Wall -Wextra -Wpedantic)

# Force colors in Ninja: https://github.com/ninja-build/ninja/wiki/FAQ
if("${CMAKE_GENERATOR}" MATCHES "Ninja")
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(cherry_blazer_flags INTERFACE -fdiagnostics-color=always)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(cherry_blazer_flags INTERFACE -fcolor-diagnostics)
    endif()
endif()

option(CHERRY_BLAZER_DEBUG "Enable debug symbols" OFF)
add_library(cherry_blazer_flags_debug INTERFACE)
target_compile_options(cherry_blazer_flags_debug INTERFACE -g)

option(CHERRY_BLAZER_NDEBUG "Disable asserts" OFF)
add_library(cherry_blazer_flags_ndebug INTERFACE)
target_compile_definitions(cherry_blazer_flags_ndebug INTERFACE NDEBUG)

option(CHERRY_BLAZER_OPTIMIZE "Enable optimizations" OFF)
add_library(cherry_blazer_flags_optimize INTERFACE)
target_compile_options(cherry_blazer_flags_optimize INTERFACE -O2)

option(CHERRY_BLAZER_ASAN "Enable address sanitizer" OFF)
add_library(cherry_blazer_flags_asan INTERFACE)
target_compile_options(cherry_blazer_flags_asan INTERFACE -fsanitize=address
                                                          -fno-omit-frame-pointer)
target_link_options(cherry_blazer_flags_asan INTERFACE -fsanitize=address)

if(CHERRY_BLAZER_DEBUG)
    if(NOT CHERRY_BLAZER_OPTIMIZE)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("-Og" cherry_blazer_compiler_has_og)
        if(cherry_blazer_compiler_has_og)
            target_compile_options(cherry_blazer_flags_debug INTERFACE -Og)
        endif()
        target_link_libraries(cherry_blazer_flags INTERFACE cherry_blazer_flags_debug)
    else()
        # Optimization flags will clash.
    endif()
endif()

if(CHERRY_BLAZER_NDEBUG)
    target_link_libraries(cherry_blazer_flags INTERFACE cherry_blazer_flags_ndebug)
endif()

if(CHERRY_BLAZER_OPTIMIZE)
    target_link_libraries(cherry_blazer_flags INTERFACE cherry_blazer_flags_optimize)
endif()

if(CHERRY_BLAZER_ASAN)
    target_link_libraries(cherry_blazer_flags INTERFACE cherry_blazer_flags_asan)
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-src")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_deps")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -DCHERRY_BLAZER_SOURCE_DIR='${CMAKE_CURRENT_SOURCE_DIR}' -P
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_googletest.cmake"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_deps")

    set(command git describe --long --tags)
    execute_process(
        COMMAND ${command}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-src"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE out
        ERROR_VARIABLE out
        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
    if(NOT "${result}" EQUAL "0")
        message(FATAL_ERROR "execute_process: '${command}' failed with output:\n${out}")
    endif()
    message(STATUS "googletest version: ${out}")
endif()

option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-src"
                 "${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-build")

add_subdirectory(src)

if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    enable_testing()

    # Set of flags built up piecewise to be used to build tests defined by this project. For tests,
    # they stack (through libcherryblazer) on top of cherry_blazer_flags.
    add_library(cherry_blazer_test_flags INTERFACE)

    # To silence clang-tidy warnings from IDE, #pragma ide is used, which compilers complain about.
    target_compile_options(cherry_blazer_test_flags INTERFACE -Wno-unknown-pragmas)

    add_subdirectory(test)
endif()

# * TODO: use FETCHCONTENT_FULLY_DISCONNECTED with previously downloaded dependencies
# * TODO: perhaps disable C in googletest (using cmake script mode)
# * TODO: perhaps googletest doesn't need python if building without gmock
# * TODO: use FETCHCONTENT_QUIET OFF on CI build
# * TODO: specify external dependencies. for example, git 1.6.5+ for ExternalProject (via
#   FetchContent)
# * TODO: CI: create configurations for different kinds of builds: w/ or wo/ flags
# * TODO: add check target (will be useful for compile-time tests too)
# * TODO: add cmake presets
