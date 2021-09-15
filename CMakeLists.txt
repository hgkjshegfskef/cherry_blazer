cmake_minimum_required(VERSION 3.17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(cherry_blazer_util)

project(cherry_blazer LANGUAGES CXX)

# ##################################################################################################
# Build preconditions

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

# ##################################################################################################
# Options

option(CHERRY_BLAZER_CI "Set ON if building in CI" OFF)

option(CHERRY_BLAZER_DEBUG "Enable debug symbols" OFF)
add_library(cherry_blazer_flags_debug INTERFACE)
target_compile_options(cherry_blazer_flags_debug INTERFACE -g)
target_compile_definitions(cherry_blazer_flags_debug INTERFACE CHERRY_BLAZER_DEBUG)

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

set(CHERRY_BLAZER_BOOST_VERSION
    "boost-1.77.0"
    CACHE STRING "Boost version")

# Dependency order is maintained: latter library may depend on former libraries.
list(
    APPEND
    boost_libraries
    pfr
    mp11
    preprocessor
    config
    io
    static_assert
    type_traits
    concept_check
    assert
    throw_exception
    core
    logic
    integer
    safe_numerics)

# ;-list is transformed into space-delimited list in order:
#
# 1. To provide user-friendly customization option (;-list looks scary).
# 2. To pass the list through execute_process() into CMake script (it is not possible to make ;-list
#    be treated like single argument).
string(REPLACE ";" " " boost_libraries_space_delimited "${boost_libraries}")

set(CHERRY_BLAZER_BOOST_LIBRARIES
    "${boost_libraries_space_delimited}"
    CACHE STRING "List of Boost libraries to use")

# ##################################################################################################
# CMAKE_ settings

# Compilation database for code completion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MESSAGE_CONTEXT_SHOW ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_EXTENSIONS OFF)

# ##################################################################################################

# Set of flags built up piecewise to be used to build sources of every target defined by this
# project.
add_library(cherry_blazer_flags INTERFACE)

target_compile_options(
    cherry_blazer_flags
    INTERFACE
        -std=c++20
        -Wall
        -Wextra
        -pedantic-errors # issue errors on standard non-compliance
        -Werror=missing-field-initializers # partial ctors is an error
        -Wdelete-non-virtual-dtor # https://wiki.sei.cmu.edu/confluence/display/cplusplus/OOP52-CPP.+Do+not+delete+a+polymorphic+object+without+a+virtual+destructor
        -Wconversion
        -Wsign-conversion)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    target_compile_options(
        cherry_blazer_flags
        INTERFACE
            -Wcast-align=strict # https://stackoverflow.com/a/25795795/10986054
                                # https://trust-in-soft.com/blog/2020/04/06/gcc-always-assumes-aligned-pointer-accesses/
            -Wshadow=compatible-local # don't warn about parameters and incompatible local types
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(
        cherry_blazer_flags INTERFACE -Wcast-align # clang doesn't have =strict
                                      -Wshadow # clang doesn't warn about parameters needlessly
    )
endif()

# Force colors in Ninja: https://github.com/ninja-build/ninja/wiki/FAQ
if("${CMAKE_GENERATOR}" MATCHES "Ninja")
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(cherry_blazer_flags INTERFACE -fdiagnostics-color=always)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(cherry_blazer_flags INTERFACE -fcolor-diagnostics)
    endif()
endif()

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

# ##################################################################################################
# googletest

set(cmake_definitions -DCHERRY_BLAZER_SOURCE_DIR='${CMAKE_CURRENT_SOURCE_DIR}')
cherry_blazer_dependency_download(googletest "${cmake_definitions}")
cherry_blazer_dependency_apply_patch(googletest googletest.patch)

option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)

list(APPEND CMAKE_MESSAGE_CONTEXT "googletest")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-src"
                 "${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-build")
list(POP_BACK CMAKE_MESSAGE_CONTEXT)

# ##################################################################################################
# boost

set(cmake_definitions
    -DCHERRY_BLAZER_SOURCE_DIR='${CMAKE_CURRENT_SOURCE_DIR}'
    -DCHERRY_BLAZER_BOOST_VERSION='${CHERRY_BLAZER_BOOST_VERSION}'
    -DCHERRY_BLAZER_BOOST_LIBRARIES='${CHERRY_BLAZER_BOOST_LIBRARIES}')
cherry_blazer_dependency_download(boost "${cmake_definitions}")

list(TRANSFORM boost_libraries PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/_deps/boost-src/libs/"
                                       OUTPUT_VARIABLE boost_include_dirs)
list(TRANSFORM boost_include_dirs APPEND "/include")
add_library(boost_headers INTERFACE)
target_include_directories(boost_headers SYSTEM INTERFACE ${boost_include_dirs})
add_library(Boost::headers ALIAS boost_headers)

# ##################################################################################################

add_subdirectory(src)

if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    enable_testing()

    # Set of flags built up piecewise to be used to build tests defined by this project. For tests,
    # they stack (through libcherryblazer) on top of cherry_blazer_flags.
    add_library(cherry_blazer_test_flags INTERFACE)

    # To silence clang-tidy warnings from IDE, #pragma ide is used, which compilers complain about.
    target_compile_options(cherry_blazer_test_flags INTERFACE -Wno-unknown-pragmas)

    target_compile_definitions(cherry_blazer_test_flags INTERFACE CHERRY_BLAZER_TEST)

    add_subdirectory(test)
endif()

# * TODO: specify external dependencies. for example, git 1.6.5+ for ExternalProject (via
#   FetchContent)
# * TODO: CI: create configurations for different kinds of builds: w/ or wo/ flags
# * TODO: add check target (will be useful for compile-time tests too)
# * TODO: add cmake presets
