add_library(libcherryblazer point.cc tuple.cc vector.cc)
target_include_directories(libcherryblazer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(libcherryblazer PUBLIC cherry_blazer_flags_final m)

add_executable(cherry_blazer main.cc)
target_link_libraries(cherry_blazer PRIVATE libcherryblazer)
