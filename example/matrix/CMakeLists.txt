# CMake runs in two phase : configuration and build. CONFIGURE_DEPENDS only evaluates at configuration.
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(SRC ${EXAMPLE_SOURCES})
    get_filename_component(EXE_NAME ${SRC} NAME_WE)
    get_filename_component(DIR ${SRC} DIRECTORY)
    get_filename_component(DIR_NAME "${DIR}" NAME)
    set(TARGET "${DIR_NAME}_${EXE_NAME}")
    message(STATUS "Adding example: ${DIR_NAME}_${EXE_NAME}")
    add_executable(${TARGET} "${SRC}")
    target_include_directories(${TARGET}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/src
        )
endforeach()