cmake_minimum_required(VERSION 3.15) # whatever

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
    GIT_TAG origin/master)
FetchContent_MakeAvailable(googletest)

add_library(cherry_blazer_default_compile_options INTERFACE)
target_compile_options(cherry_blazer_default_compile_options INTERFACE -Wall -Wextra -pedantic)

option(DEBUG OFF)
add_library(cherry_blazer_debug_compile_options INTERFACE)
target_compile_options(cherry_blazer_debug_compile_options INTERFACE -g)

option(NDEBUG OFF)
add_library(cherry_blazer_ndebug_compile_options INTERFACE)
target_compile_options(cherry_blazer_ndebug_compile_options INTERFACE -O2)
target_compile_definitions(cherry_blazer_ndebug_compile_options INTERFACE NDEBUG)

option(ASAN OFF)
add_library(cherry_blazer_asan_compile_options INTERFACE)
target_compile_options(cherry_blazer_asan_compile_options INTERFACE -fsanitize=address
                                                                    -fno-omit-frame-pointer)
target_link_options(cherry_blazer_asan_compile_options INTERFACE -fsanitize=address)

# ##################################################################################################
add_executable(cherry_blazer main.cc)
# ##################################################################################################
target_include_directories(cherry_blazer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(cherry_blazer PRIVATE cherry_blazer_default_compile_options m)

if(DEBUG AND NDEBUG)
    message(FATAL_ERROR "Cannot have both DEBUG and NDEBUG set.")
elseif(DEBUG)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_debug_compile_options)
elseif(NDEBUG)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_ndebug_compile_options)
endif()

if(ASAN)
    target_link_libraries(cherry_blazer PRIVATE cherry_blazer_asan_compile_options)
endif()

enable_testing()
add_library(cherry_blazer_testing_default_compile_options INTERFACE)
target_compile_options(cherry_blazer_testing_default_compile_options INTERFACE -Wno-unknown-pragmas)

# ##################################################################################################
add_executable(cherry_blazer_test point.cc point_test.cc tuple.cc tuple_test.cc vector.cc
                                  vector_test.cc)
# ##################################################################################################
target_link_libraries(cherry_blazer_test PRIVATE cherry_blazer_testing_default_compile_options
                                                 gtest_main)

include(GoogleTest)
gtest_discover_tests(cherry_blazer_test)
