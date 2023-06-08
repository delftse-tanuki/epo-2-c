#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#define NK_INCLUDE_FIXED_TYPES
#include "../thirdparty/nuklear.h"

#define DEFAULT_WINDOW_WIDTH 1000
#define DEFAULT_WINDOW_HEIGHT 900
#include "../thirdparty/nuklear_glfw_gl2.h"

#include "gui.h"

static void error_callback(int e, const char *d) { printf("Error %d: %s\n", e, d); }

int main(void) {
    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GLFW] failed to init!\n");
        exit(1);
    }
    state.window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "EPO-2", NULL, NULL);
    glfwMakeContextCurrent(state.window);
    glfwGetWindowSize(state.window, &state.window_width, &state.window_height);

    /* GUI */
    state.ctx = nk_glfw3_init(state.window, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        struct nk_font *franklin = nk_font_atlas_add_from_file(atlas, "libre_franklin.ttf", 18, 0);
        nk_glfw3_font_stash_end();
        nk_style_set_font(state.ctx, &franklin->handle);
    }

    state.robot_state.facing = NORTH;
    state.robot_state.next_instruction = FORWARD;
    state.robot_state.data_received = false;
    state.robot_state.last_reported_position.x = -1;
    state.robot_state.last_reported_position.y = 1;
    state.robot_state.mines_count = 0;
    state.robot_state.ignore_mines = false;

    gui_initialize();

    // Main loop.
    while (!glfwWindowShouldClose(state.window)) {
        gui_update();
    }

    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}