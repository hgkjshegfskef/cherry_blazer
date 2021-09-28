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
target_compile_options(cherry_blazer_flags_debug INTERFACE -g -fno-omit-frame-pointer)
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

option(CHERRY_BLAZER_LTO "Enable LTO" OFF)

option(CHERRY_BLAZER_LLD "Use lld" OFF)

set(CHERRY_BLAZER_BOOST_VERSION
    "1.77.0"
    CACHE STRING "Boost version")

set(CHERRY_BLAZER_USE_CONSTEXPR
    OFF
    CACHE BOOL "Calculate most things at compile-time.")

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
        cherry_blazer_flags
        INTERFACE
            -Wcast-align # clang doesn't have =strict
            -Wshadow # clang doesn't warn about parameters needlessly
            -fconstexpr-steps=4294967295 # uint32_t max, because clang doesn't optimize constexpr.
                                         # if this doesn't work (I mean, /when/ this stops working),
                                         # one can patch clang to use uint64.
            -fconstexpr-backtrace-limit=0 # to see what code led to "too many steps" above
    )
    add_compile_options(-stdlib=libc++) # feature parity with compiler
    add_link_options(-stdlib=libc++ -lc++abi)
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
        target_compile_options(cherry_blazer_flags_debug INTERFACE -O0)
        target_link_libraries(cherry_blazer_flags INTERFACE cherry_blazer_flags_debug)
    else()
        # Optimization flags will clash.
    endif()
endif()

if(CHERRY_BLAZER_LLD)
    target_link_options(cherry_blazer_flags INTERFACE -fuse-ld=lld)
endif()

if(CHERRY_BLAZER_LTO)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options(cherry_blazer_flags INTERFACE -flto=auto -fwhole-program
                                                             -fno-fat-lto-objects)
        target_link_options(cherry_blazer_flags INTERFACE -flto=auto -fwhole-program
                            -fno-fat-lto-objects)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(cherry_blazer_flags INTERFACE -flto)
        target_link_options(cherry_blazer_flags INTERFACE -flto)
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

cherry_blazer_dependency_apply_patch(googletest thirdparty-patches/googletest.patch)

option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    option(GTEST_USE_LIBCPP ON)
endif()

list(APPEND CMAKE_MESSAGE_CONTEXT "googletest")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-src"
                 "${CMAKE_CURRENT_SOURCE_DIR}/_deps/googletest-build")
list(POP_BACK CMAKE_MESSAGE_CONTEXT)

# ##################################################################################################
# boost

set(cmake_definitions -DCHERRY_BLAZER_SOURCE_DIR='${CMAKE_SOURCE_DIR}'
                      -DCHERRY_BLAZER_BOOST_VERSION='${CHERRY_BLAZER_BOOST_VERSION}')
cherry_blazer_dependency_download(boost "${cmake_definitions}")

cherry_blazer_dependency_apply_patch(boost thirdparty-patches/boost.patch)

cherry_blazer_dependency_configure(
    boost "./bootstrap.sh --prefix=../boost-install --with-icu --with-python=python3"
    "${CMAKE_SOURCE_DIR}/_deps/boost-src")

include(ProcessorCount)
ProcessorCount(n)
cherry_blazer_dependency_install(
    boost "./b2 --build-dir=../boost-build --user-config=boostbuild.sucks install \
variant=release debug-symbols=on threading=multi runtime-link=shared link=shared,static \
--jobs=${n}" "${CMAKE_SOURCE_DIR}/_deps/boost-src")

set(Boost_ROOT "${CMAKE_SOURCE_DIR}/_deps/boost-install")
find_package(Boost ${CHERRY_BLAZER_BOOST_VERSION} CONFIG REQUIRED)

# ~~~
# To list all boost targets:
# find_package(Boost ${CHERRY_BLAZER_BOOST_VERSION} CONFIG REQUIRED ALL)
# message(WARNING "All Boost targets: ${Boost_ALL_TARGETS}")
# ~~~

# ##################################################################################################
# markable (compact_optional)

set(cmake_definitions -DCHERRY_BLAZER_SOURCE_DIR='${CMAKE_CURRENT_SOURCE_DIR}')
cherry_blazer_dependency_download(markable "${cmake_definitions}")

add_library(markable INTERFACE)
target_include_directories(markable INTERFACE SYSTEM
                                              "${CMAKE_SOURCE_DIR}/_deps/markable-src/include")
add_library(ak_toolkit::markable ALIAS markable)

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
