add_library(
    libcherryblazer
    canvas.cc
    color.cc
    intersection.cc
    mat4d.cc
    mat4f.cc
    point3d.cc
    point3f.cc
    ppm.cc
    ray.cc
    sphere.cc
    transformation.cc
    vec3d.cc
    vec3f.cc)

# Use "" includes in implementation files, but users will use <cherry_blazer/> includes in their
# code. NOTE: expects config.hh in CMAKE_CURRENT_BINARY_DIR
target_include_directories(
    libcherryblazer
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/..")

target_link_libraries(libcherryblazer PUBLIC cherry_blazer_flags m Boost::headers
                                             ak_toolkit::markable fmt::fmt)
