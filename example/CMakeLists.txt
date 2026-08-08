# Find every .cpp file under examples/

# CMake runs in two phase : configuration and build. CONFIGURE_DEPENDS only evaluates at configuration.
# file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.cpp")

# foreach(SRC ${EXAMPLE_SOURCES})
#     get_filename_component(EXE_NAME ${SRC} NAME_WE)
#     get_filename_component(DIR ${SRC} DIRECTORY)
#     get_filename_component(DIR_NAME "${DIR}" NAME)
#     set(TARGET "${DIR_NAME}_${EXE_NAME}")
#     message(STATUS "Adding example: ${DIR_NAME}_${EXE_NAME}")
#     add_executable(${TARGET} "${SRC}")
# endforeach()

# Look for CMakeLists.txt recursively within this folder and get their full path
file(GLOB_RECURSE example_cmakelists
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt"
)

foreach(cmake_file IN LISTS example_cmakelists)
    get_filename_component(example_dir "${cmake_file}" DIRECTORY)
    message(STATUS "Adding subdirectory: ${example_dir}")
    add_subdirectory("${example_dir}")
endforeach()
