# CMAKE_CURRENT_SOURCE_DIR = full path to this folder, e.g. .../../hello_world
# NAME mode strips it down to just "hello_world"
get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${CURRENT_DIR_NAME}
    main.cpp
)
