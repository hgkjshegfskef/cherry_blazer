# Configure the library. This config is includable in user code as <cherry_blazer/config.hh>
configure_file(cherry_blazer_config.hh.in "${CMAKE_CURRENT_BINARY_DIR}/cherry_blazer/config.hh")

# Add the library first, so that other targets can link to it.
add_subdirectory(cherry_blazer)

add_subdirectory(projectile)
add_subdirectory(clock)
add_subdirectory(silhouette)
add_subdirectory(figure)
