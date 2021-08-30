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

if(CMAKE_CXX_CLANG_TIDY)
    # Deselect GTest targets from being checked by clang-tidy.
    set_target_properties(gtest gtest_main PROPERTIES CXX_CLANG_TIDY "")
endif()
if(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # Deselect compilation tests from being checked by include-what-you-use.
    set_target_properties(gtest gtest_main PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
endif()

add_executable(cherry_blazer_test point_test.cc tuple_test.cc vector_test.cc)
target_link_libraries(cherry_blazer_test PRIVATE libcherryblazer GTest::gtest_main
                                                 cherry_blazer_test_flags)

include(GoogleTest)
gtest_discover_tests(cherry_blazer_test)

function(cherry_blazer_add_compilation_test test_name test_contents)
    set(test_path "${CMAKE_BINARY_DIR}/test/compilation-src/${test_name}.cc")

    file(WRITE "${test_path}" "${test_contents}")

    # CMake doesn't have a notion of compile-time testing, only run-time, so compile and link the
    # test into a static library as a workaround.
    add_library(${test_name} STATIC EXCLUDE_FROM_ALL "${test_path}")
    target_link_libraries(${test_name} PRIVATE cherry_blazer_test_flags libcherryblazer)

    set_target_properties(
        ${test_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/compilation-bin"
                                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/compilation-bin")

    if(CMAKE_CXX_CLANG_TIDY)
        # Deselect compilation tests from being checked by clang-tidy.
        set_target_properties(${test_name} PROPERTIES CXX_CLANG_TIDY "")
    endif()
    if(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
        # Deselect compilation tests from being checked by include-what-you-use.
        set_target_properties(${test_name} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
    endif()

    if(CHERRY_BLAZER_CI)
        # See which command compiled the test.
        add_test(NAME ${test_name} COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
                                           --target ${test_name} -- VERBOSE=1)
    else()
        add_test(NAME ${test_name} COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
                                           --target ${test_name})
    endif()

    # The purpose of (most) compilation tests is to test that something fails to compile.
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL 1)
endfunction()

include(compilation/point_test.cmake)
include(compilation/vector_test.cmake)
