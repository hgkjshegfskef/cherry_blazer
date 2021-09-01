add_library(libcherryblazer color.cc point.cc vector.cc)
target_include_directories(libcherryblazer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libcherryblazer PUBLIC m cherry_blazer_flags)

add_executable(cherry_blazer projectile.cc projectile_main.cc)
target_link_libraries(cherry_blazer PRIVATE libcherryblazer)
