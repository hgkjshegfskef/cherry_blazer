add_library(libcherryblazer canvas.cc color.cc point.cc vector.cc)
target_include_directories(libcherryblazer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libcherryblazer PUBLIC Boost::pfr m cherry_blazer_flags)
if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    target_link_libraries(libcherryblazer PUBLIC GTest::gtest_main)
endif()

add_executable(cherry_blazer projectile.cc projectile_main.cc)
target_link_libraries(cherry_blazer PRIVATE libcherryblazer)
