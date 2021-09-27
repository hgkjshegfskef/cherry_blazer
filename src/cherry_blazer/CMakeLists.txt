add_library(libcherryblazer canvas.cc color.cc mat4d.cc ppm.cc vec3d.cc)

# Use "" includes in implementation files, but users will use <cherry_blazer/> includes in their
# code. NOTE: expects config.hh in CMAKE_CURRENT_BINARY_DIR
target_include_directories(
    libcherryblazer
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/..")

target_link_libraries(libcherryblazer PUBLIC cherry_blazer_flags m Boost::headers)
if(BUILD_TESTING OR CHERRY_BLAZER_TEST)
    target_link_libraries(libcherryblazer PUBLIC GTest::gtest_main)
endif()
