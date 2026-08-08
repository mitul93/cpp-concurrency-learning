get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${CURRENT_DIR_NAME}
    main.cpp
)
