# * target_link_options: 3.13
cmake_minimum_required(VERSION 3.15)

project(cherry_blazer LANGUAGES CXX)

if(CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Don't set CMAKE_BUILD_TYPE for this project.")
endif()

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(FATAL_ERROR "Direct inclusion of cherry_blazer as a subproject is not supported.")
endif()

if(NOT (UNIX AND NOT APPLE))
    message(FATAL_ERROR "cherry_blazer only supports Linux.")
endif()

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
message("Downloading Google Test...")
FetchContent_MakeAvailable(googletest)

add_library(cherry_blazer_default_compile_options INTERFACE)
target_compile_options(cherry_blazer_default_compile_options INTERFACE -Wall -Wextra -pedantic)

option(CHERRY_BLAZER_DEBUG "Enable debug symbols" OFF)
add_library(cherry_blazer_debug_compile_options INTERFACE)
target_compile_options(cherry_blazer_debug_compile_options INTERFACE -g)

option(CHERRY_BLAZER_NDEBUG "Disable asserts" OFF)
add_library(cherry_blazer_ndebug_compile_options INTERFACE)
target_compile_definitions(cherry_blazer_ndebug_compile_options INTERFACE NDEBUG)

option(CHERRY_BLAZER_OPTIMIZE "Enable optimizations" OFF)
add_library(cherry_blazer_optimize_compile_options INTERFACE)
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Og" cherry_blazer_compiler_has_og)
if(CHERRY_BLAZER_DEBUG AND cherry_blazer_compiler_has_og)
    target_compile_options(cherry_blazer_optimize_compile_options INTERFACE -Og)
else()
    target_compile_options(cherry_blazer_optimize_compile_options INTERFACE -O2)
endif()

option(CHERRY_BLAZER_ASAN "Enable address sanitizer" OFF)
add_library(cherry_blazer_asan_compile_options INTERFACE)
target_compile_options(cherry_blazer_asan_compile_options INTERFACE -fsanitize=address
                                                                    -fno-omit-frame-pointer)
target_link_options(cherry_blazer_asan_compile_options INTERFACE -fsanitize=address)

# ##################################################################################################
add_executable(cherry_blazer main.cc)
# ##################################################################################################
target_include_directories(cherry_blazer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(cherry_blazer PRIVATE cherry_blazer_default_compile_options m)

if(CHERRY_BLAZER_DEBUG)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_debug_compile_options)
endif()
if(CHERRY_BLAZER_NDEBUG)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_ndebug_compile_options)
endif()
if(CHERRY_BLAZER_OPTIMIZE)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_optimize_compile_options)
endif()
if(CHERRY_BLAZER_ASAN)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_asan_compile_options)
endif()

if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    enable_testing()
    add_library(cherry_blazer_testing_default_compile_options INTERFACE)
    target_compile_options(cherry_blazer_testing_default_compile_options
                           INTERFACE -Wno-unknown-pragmas)

    # ##############################################################################################
    add_executable(cherry_blazer_test point.cc point_test.cc tuple.cc tuple_test.cc vector.cc
                                      vector_test.cc)
    # ##############################################################################################
    target_link_libraries(
        cherry_blazer_test PRIVATE cherry_blazer_default_compile_options
                                   cherry_blazer_testing_default_compile_options gtest_main)

    include(GoogleTest)
    gtest_discover_tests(cherry_blazer_test)
endif()

# * TODO: rename cherry_blazer_*_compile_options to cherry_blazer_compile_options_*
# * TODO: use FETCHCONTENT_FULLY_DISCONNECTED with previously downloaded dependencies
# * TODO: perhaps disable C in googleproject (using cmake script mode)
# * TODO: use FETCHCONTENT_QUIET OFF on CI build
# * TODO: specify external dependencies. for example, git 1.6.5+ for ExternalProject (via
#   FetchContent)
