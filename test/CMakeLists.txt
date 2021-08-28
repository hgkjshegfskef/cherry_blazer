add_executable(cherry_blazer_test point_test.cc tuple_test.cc vector_test.cc)
target_link_libraries(cherry_blazer_test PRIVATE libcherryblazer GTest::gtest_main
                                                 cherry_blazer_test_flags)

include(GoogleTest)
gtest_discover_tests(cherry_blazer_test)

function(cherry_blazer_add_compilation_test test_name test_contents)
    set(test_path "${CMAKE_BINARY_DIR}/test/compilation-src/${test_name}.cc")
    file(WRITE "${test_path}" "${test_contents}")
    add_library(${test_name} STATIC EXCLUDE_FROM_ALL "${test_path}")
    set_target_properties(
        ${test_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/compilation-bin"
                                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/compilation-bin")
    target_link_libraries(${test_name} PRIVATE cherry_blazer_test_flags libcherryblazer)
    add_test(NAME ${test_name} COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target
                                       ${test_name})
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL 1)
endfunction()

include(compilation/point_test.cmake)
include(compilation/vector_test.cmake)
