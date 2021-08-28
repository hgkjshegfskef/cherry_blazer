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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG origin/master
    GIT_PROGRESS ON)
set(BUILD_GMOCK
    OFF
    CACHE BOOL "" FORCE)
set(INSTALL_GTEST
    OFF
    CACHE BOOL "" FORCE)
message(STATUS "Downloading Google Test...")
FetchContent_MakeAvailable(googletest)

# Default set of flags used to build sources of every target defined by this project.
add_library(cherry_blazer_flags_default INTERFACE)
target_compile_options(cherry_blazer_flags_default INTERFACE -Wall -Wextra -pedantic)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    target_compile_options(cherry_blazer_flags_default INTERFACE -fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(cherry_blazer_flags_default INTERFACE -fcolor-diagnostics)
endif()

# Final set of flags used to build sources of every target defined by this project.
add_library(cherry_blazer_flags_final INTERFACE)
# Other flags will be collected throughout the listfiles and linked to the final flags.
# Consequently, targets defined by this project only link to the final flags.
target_link_libraries(cherry_blazer_flags_final INTERFACE cherry_blazer_flags_default)

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
        target_link_libraries(cherry_blazer_flags_final INTERFACE cherry_blazer_flags_debug)
    else()
        # Optimization flags will clash.
    endif()
endif()

if(CHERRY_BLAZER_NDEBUG)
    target_link_libraries(cherry_blazer_flags_final INTERFACE cherry_blazer_flags_ndebug)
endif()

if(CHERRY_BLAZER_OPTIMIZE)
    target_link_libraries(cherry_blazer_flags_final INTERFACE cherry_blazer_flags_optimize)
endif()

if(CHERRY_BLAZER_ASAN)
    target_link_libraries(cherry_blazer_flags_final INTERFACE cherry_blazer_flags_asan)
endif()

add_subdirectory(src)

if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    enable_testing()

    # Default set of flags used to build tests defined by this project.
    add_library(cherry_blazer_test_flags_default INTERFACE)
    target_compile_options(cherry_blazer_test_flags_default INTERFACE -Wno-unknown-pragmas)

    # Final set of flags used to build tests defined by this project.
    add_library(cherry_blazer_test_flags_final INTERFACE)
    # Other test flags will be collected throughout the listfiles and linked to the final test
    # flags. Consequently, tests defined by this project only link to the final test flags and the
    # final flags for every target defined by this project.
    target_link_libraries(cherry_blazer_test_flags_final INTERFACE cherry_blazer_test_flags_default)

    add_subdirectory(test)
endif()

# * TODO: rename cherry_blazer_*_compile_options to cherry_blazer_compile_options_*
# * TODO: use FETCHCONTENT_FULLY_DISCONNECTED with previously downloaded dependencies
# * TODO: perhaps disable C in googletest (using cmake script mode)
# * TODO: perhaps googletest doesn't need python if building without gmock
# * TODO: use FETCHCONTENT_QUIET OFF on CI build
# * TODO: specify external dependencies. for example, git 1.6.5+ for ExternalProject (via
#   FetchContent)
# * TODO: CI: create configurations for different kinds of builds: w/ or wo/ flags
# * TODO: add check target (will be useful for compile-time tests too)
# * TODO: add cmake presets
