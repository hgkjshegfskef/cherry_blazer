add_executable(projectile main.cc projectile.cc)
target_include_directories(projectile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(projectile PRIVATE libcherryblazer)
