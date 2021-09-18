configure_file(config.hh.in config.hh)

add_library(libcherryblazer canvas.cc color.cc ppm.cc)
target_include_directories(libcherryblazer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                                  "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(libcherryblazer PUBLIC cherry_blazer_flags m Boost::headers)
if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    target_link_libraries(libcherryblazer PUBLIC GTest::gtest_main)
endif()

add_executable(cherry_blazer projectile.cc projectile_main.cc)
target_link_libraries(cherry_blazer PRIVATE libcherryblazer)
