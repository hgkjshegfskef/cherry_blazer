function(cherry_blazer_dependency_download dependency_name cmake_definitions)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src")
    file(GLOB empty "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src/*")
    list(LENGTH empty is_empty)
    if(is_empty EQUAL 0)
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

function(cherry_blazer_dependency_apply_patch dependency_name patch_path)
    get_filename_component(patch_realpath "${patch_path}" REALPATH)
    get_filename_component(patch_name "${patch_realpath}" NAME)

    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src/${patch_name}")
        message(STATUS "Copying ${patch_name}")
        file(COPY "${patch_path}" DESTINATION "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src")
        message(STATUS "Copying ${patch_name} - done")

        message(STATUS "Applying ${patch_name}")
        set(command git apply ${patch_name})
        execute_process(
            COMMAND ${command} COMMAND_ECHO STDOUT
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/_deps/${dependency_name}-src"
            RESULT_VARIABLE result)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed.")
        endif()
        message(STATUS "Applying ${patch_name} - done")
    endif()
endfunction()

function(cherry_blazer_dependency_configure dep_name configure_command work_dir)
    # * Prerequisites: src_dir exists and is populated.
    # * Acts when: build_dir either doesn't exist, or is empty.

    # Note: why the "either, or"? Because FetchContent creates build_dir in automatic fashion
    # (therefore, it exists, yet is empty). Additionally, it can be useful to trigger this routine
    # by removing build_dir manually.

    set(deps_dir "${CMAKE_SOURCE_DIR}/_deps")
    set(src_dir "${deps_dir}/${dep_name}-src")
    set(build_dir "${deps_dir}/${dep_name}-build")

    file(GLOB empty "${src_dir}/*")
    list(LENGTH empty is_empty)
    if((NOT EXISTS "${src_dir}") OR ("${is_empty}" EQUAL "0"))
        message(FATAL_ERROR "'${src_dir}' must exist and be non-empty.")
    endif()

    file(GLOB empty "${build_dir}/*")
    list(LENGTH empty is_empty)
    if((NOT EXISTS "${build_dir}") OR ("${is_empty}" EQUAL "0"))
        file(MAKE_DIRECTORY "${build_dir}")

        message(STATUS "Configuring ${dep_name}")

        string(REPLACE " " ";" command "${configure_command}")
        execute_process(
            COMMAND ${command} COMMAND_ECHO STDOUT
            WORKING_DIRECTORY "${work_dir}"
            RESULT_VARIABLE result)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed.")
        endif()

        message(STATUS "Configuring ${dep_name} - done")
    endif()
endfunction()

function(cherry_blazer_dependency_build dep_name build_command work_dir)
    # * Expectations: src_dir exists and is populated, build_dir exists.
    # * Acts when: build_dir is empty.

    # Note: to trigger this routine, remove build_dir/*, but not build_dir.

    set(deps_dir "${CMAKE_SOURCE_DIR}/_deps")
    set(src_dir "${deps_dir}/${dep_name}-src")
    set(build_dir "${deps_dir}/${dep_name}-build")

    file(GLOB empty "${src_dir}/*")
    list(LENGTH empty is_empty)
    if((NOT EXISTS "${src_dir}") OR ("${is_empty}" EQUAL "0"))
        message(FATAL_ERROR "'${src_dir}' must exist and be non-empty.")
    endif()

    if(NOT EXISTS "${build_dir}")
        message(FATAL_ERROR "'${build_dir}' must exist.")
    endif()

    file(GLOB empty "${build_dir}/*")
    list(LENGTH empty is_empty)
    if("${is_empty}" EQUAL "0")
        message(STATUS "Building ${dep_name}")

        string(REPLACE " " ";" command "${build_command}")
        execute_process(
            COMMAND ${command} COMMAND_ECHO STDOUT
            WORKING_DIRECTORY "${work_dir}"
            RESULT_VARIABLE result)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed.")
        endif()

        message(STATUS "Building ${dep_name} - done")
    endif()
endfunction()

function(cherry_blazer_dependency_install dep_name install_command work_dir)
    # * Expectations: src_dir exists and is populated, build_dir exists, but can be empty,
    #   install_dir either doesn't exist, or is empty.
    # * Acts when: install_dir is empty.

    # * Note: build_dir can be empty for the case when build and install is one command.
    # * Note: unlike in configure step case, install_dir is "either, or" for convenience. One can
    #   trigger this routine by either removing install_dir or its contents.

    set(deps_dir "${CMAKE_SOURCE_DIR}/_deps")
    set(src_dir "${deps_dir}/${dep_name}-src")
    set(build_dir "${deps_dir}/${dep_name}-build")
    set(install_dir "${deps_dir}/${dep_name}-install")

    file(GLOB empty "${src_dir}/*")
    list(LENGTH empty is_empty)
    if((NOT EXISTS "${src_dir}") OR ("${is_empty}" EQUAL "0"))
        message(FATAL_ERROR "'${src_dir}' must exist and be non-empty.")
    endif()

    if(NOT EXISTS "${build_dir}")
        message(FATAL_ERROR "'${build_dir}' must exist.")
    endif()

    file(GLOB empty "${install_dir}/*")
    list(LENGTH empty is_empty)
    if((NOT EXISTS "${install_dir}") OR ("${is_empty}" EQUAL "0"))
        file(MAKE_DIRECTORY "${install_dir}")

        message(STATUS "Installing ${dep_name}")

        string(REPLACE " " ";" command "${install_command}")
        execute_process(
            COMMAND ${command} COMMAND_ECHO STDOUT
            WORKING_DIRECTORY "${work_dir}"
            RESULT_VARIABLE result)
        if(NOT "${result}" EQUAL "0")
            message(FATAL_ERROR "execute_process: '${command}' failed.")
        endif()

        message(STATUS "Installing ${dep_name} - done")
    endif()
endfunction()
