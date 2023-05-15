add_executable(epo2gui main.c grid.c grid.h gui_helpers.c gui_helpers.h)

find_package(OpenGL REQUIRED)
target_link_libraries(epo2gui OpenGL::GL)
target_link_libraries(epo2gui backend)
target_link_libraries(epo2gui glfw)

add_custom_command(
        TARGET epo2gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/gui/libre_franklin.ttf
        ${CMAKE_CURRENT_BINARY_DIR}/libre_franklin.ttf)