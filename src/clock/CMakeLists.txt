add_executable(clock main.cc)
target_link_libraries(clock PRIVATE libcherryblazer)

add_executable(clock_generate generate.cc)
target_link_libraries(clock_generate PRIVATE libcherryblazer fmt::fmt)
