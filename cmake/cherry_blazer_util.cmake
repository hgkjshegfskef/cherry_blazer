function(cherry_blazer_dependency_download dependency_name cmake_definitions)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src")
        file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/_deps")

        message(STATUS "Downloading ${dependency_name}")
        set(command "${CMAKE_COMMAND}" ${cmake_definitions} -P
                    "${CMAKE_SOURCE_DIR}/cmake/get_${dependency_name}.cmake")
        execute_process(COMMAND ${command} RESULT_VARIABLE result)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed")
        endif()
        message(STATUS "Downloading ${dependency_name} - done")

        set(command git describe --long --tags)
        execute_process(
            COMMAND ${command}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE out
            ERROR_VARIABLE out
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed with output:\n${out}")
        endif()
        message(STATUS "${dependency_name} version: ${out}")
    endif()
endfunction()

function(cherry_blazer_dependency_apply_patch project_name patch_path)
    get_filename_component(patch_realpath "${patch_path}" REALPATH)
    get_filename_component(patch_name "${patch_realpath}" NAME)

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/_deps/${project_name}-src/${patch_name}")
        message(STATUS "Copying ${patch_name}")
        file(COPY "${patch_path}" DESTINATION "${CMAKE_SOURCE_DIR}/_deps/${project_name}-src")
        message(STATUS "Copying ${patch_name} - done")

        message(STATUS "Applying ${patch_name}")
        set(command git apply ${patch_name})
        execute_process(
            COMMAND ${command}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_deps/${project_name}-src"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE out
            ERROR_VARIABLE out
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed with output:\n${out}")
        endif()
        message(STATUS "Applying ${patch_name} - done")
    endif()
endfunction()
