#include "gui.h"

#include <stdio.h>
#include <string.h>

#define NK_INCLUDE_FIXED_TYPES
#include "../thirdparty/nuklear.h"
#include "../thirdparty/nuklear_glfw_gl2.h"

#include "gui_helpers.h"
#include "grid.h"
#include "../backend/challenges/challenge_a.h"
#include "../backend/challenges/challenge_b.h"
#include "../backend/challenges/challenge_c.h"

struct AppState state;

struct ChallengeSetup {
    bool ready_to_start;
    struct Point station1;
    struct Point station2;
    struct Point station3;
    bool continue_challenge_c;
};

static struct ChallengeSetup challenge_setup = {false, {-1, -1}, {-1, -1}, {-1, -1}, false};

void challenge_select_gui();
void challenge_ab_setup_gui(struct Point grid_selection, bool grid_selected);
void challenge_c_setup_gui();
void challenge_progress_gui(bool* call_for_abort);
void grid_gui(struct Point *selected_point, bool *grid_selected);
void challenge_ended_callback();
struct RobotState* get_robot_state_callback();
void reset_challenge_setup();

void gui_initialize() {
    challenge_ended = challenge_ended_callback;
    get_robot_state = get_robot_state_callback;
}

void gui_update(bool* call_for_abort) {
    if (challenge_setup.ready_to_start) {
        challenge_setup.ready_to_start = false;
        state.challenge_state = CHALLENGE_IN_PROGRESS;
        switch (state.current_challenge) {
            case CHALLENGE_A:
                start_challenge_a(challenge_setup.station1, challenge_setup.station2, challenge_setup.station3);
                break;
            case CHALLENGE_B:
                start_challenge_b(challenge_setup.station1, challenge_setup.station2, challenge_setup.station3);
                break;
            case CHALLENGE_C:
                start_challenge_c();
                break;
        }
    }
    else if (challenge_setup.continue_challenge_c) {
        challenge_setup.continue_challenge_c = false;
        continue_challenge_c();
    }

    glfwPollEvents();
    nk_glfw3_new_frame();

    static struct Point grid_selection;
    static bool grid_selected = false;

    if (nk_begin(state.ctx, "EPO-2", nk_rect(0, 0, (float) state.window_width, (float) state.window_height),
                 NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE
    )) {
        vertical_spacer(state.ctx, 1);

        switch (state.challenge_state) {
            case IDLE:
                challenge_select_gui();
                break;
            case CHALLENGE_IN_PROGRESS:
                challenge_progress_gui(call_for_abort);
                break;
            case CHALLENGE_SETUP: {
                switch (state.current_challenge) {
                    case CHALLENGE_A:
                    case CHALLENGE_B:
                        challenge_ab_setup_gui(grid_selection, grid_selected);
                        break;
                    case CHALLENGE_C:
                        challenge_c_setup_gui();
                        break;
                }
                break;
            }
        }

        vertical_spacer(state.ctx, 1);

        grid_gui(&grid_selection, &grid_selected);
    }
    nk_end(state.ctx);

    if (nk_window_is_hidden(state.ctx, "EPO-2")) {
        glfwSetWindowShouldClose(state.window, GLFW_TRUE);
    }

    glfwGetWindowSize(state.window, &state.window_width, &state.window_height);
    glViewport(0, 0, state.window_width, state.window_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glfwSwapBuffers(state.window);
}

void challenge_select_gui() {
    nk_layout_row_dynamic(state.ctx, 30, 1);
    nk_label(state.ctx, "Challenge Select", NK_TEXT_ALIGN_CENTERED);

    nk_layout_row_dynamic(state.ctx, 170, 3);

    if (nk_group_begin(state.ctx, "Challenge Selection", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(state.ctx, 105, 2);
        nk_spacing(state.ctx, 1);
        if (nk_group_begin(state.ctx, "Challenges", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(state.ctx, 30, 1);
            state.current_challenge = nk_option_label(state.ctx, "Challenge A", state.current_challenge == CHALLENGE_A) ? CHALLENGE_A : state.current_challenge;
            state.current_challenge = nk_option_label(state.ctx, "Challenge B", state.current_challenge == CHALLENGE_B) ? CHALLENGE_B : state.current_challenge;
            state.current_challenge = nk_option_label(state.ctx, "Challenge C", state.current_challenge == CHALLENGE_C) ? CHALLENGE_C : state.current_challenge;
            nk_group_end(state.ctx);
        }
        nk_group_end(state.ctx);
    }

    char* label = "SET UP CHALLENGE";
    if (state.current_challenge == CHALLENGE_C) {
        label = "START CHALLENGE";
    }
    if (nk_button_label(state.ctx, label)) {
        if (state.current_challenge == CHALLENGE_C) {
            challenge_setup.ready_to_start = true;
        }
        else {
            state.challenge_state = CHALLENGE_SETUP;
        }
    }

    if (nk_group_begin(state.ctx, "Robot Status:", NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(state.ctx, 30, 1);
        if (state.robot_state.data_received) {
            nk_label_colored(state.ctx, "Signal Received", NK_TEXT_ALIGN_LEFT, nk_green);
        }
        else {
            nk_label_colored(state.ctx, "No Signal", NK_TEXT_ALIGN_LEFT, nk_red);
        }
        if (!state.robot_state.major_failure) {
            nk_label_colored(state.ctx, "All Systems OK", NK_TEXT_ALIGN_LEFT, nk_green);
        }
        else {
            nk_label_colored(state.ctx, "MAJOR FAILURE", NK_TEXT_ALIGN_LEFT, nk_red);
        }
        nk_group_end(state.ctx);
    }
}

void challenge_ab_setup_gui(struct Point grid_selection, bool grid_selected) {
    if (grid_selected && grid_selection.x == -1 && grid_selection.y != -1) {
        if (challenge_setup.station1.y == -1) {
            challenge_setup.station1 = grid_selection;
        }
        else if (challenge_setup.station2.y == -1) {
            challenge_setup.station2 = grid_selection;
        }
        else if (challenge_setup.station3.y == -1) {
            challenge_setup.station3 = grid_selection;
        }
        else {
            challenge_setup.station1 = grid_selection;
            challenge_setup.station2.y = -1;
            challenge_setup.station3.y = -1;
        }
    }

    nk_layout_row_dynamic(state.ctx, 200, 1);
    char* label = "Challenge A Setup";
    if (state.current_challenge == CHALLENGE_B) {
        label = "Challenge B Setup";
    }
    if (nk_group_begin(state.ctx, label, NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(state.ctx, 120, 2);
        if (nk_group_begin(state.ctx, "checkpoint_list", 0)) {
            nk_layout_row_dynamic(state.ctx, 30, 1);
            nk_label(state.ctx, "Please select the 3 stations for the robot to visit.", NK_TEXT_ALIGN_LEFT);
            if (challenge_setup.station1.y != -1 && challenge_setup.station2.y != -1 && challenge_setup.station3.y != -1) {
                if (nk_button_label(state.ctx, "START CHALLENGE")) {
                    challenge_setup.ready_to_start = true;
                }
            }
            nk_group_end(state.ctx);
        }
        if (nk_group_begin(state.ctx, "checkpoint_list", 0)) {
            nk_layout_row_dynamic(state.ctx, 30, 1);
            char checkpoint_text[18] = "Checkpoint 1: S12";
            char checkpoint_name_text[4] = "S12";
            get_lee_name(index_to_lee(challenge_setup.station1), checkpoint_name_text);
            sprintf(checkpoint_text, "Checkpoint 1: %s", checkpoint_name_text);
            nk_label(state.ctx, checkpoint_text, NK_TEXT_ALIGN_LEFT);
            get_lee_name(index_to_lee(challenge_setup.station2), checkpoint_name_text);
            sprintf(checkpoint_text, "Checkpoint 2: %s", checkpoint_name_text);
            nk_label(state.ctx, checkpoint_text, NK_TEXT_ALIGN_LEFT);
            get_lee_name(index_to_lee(challenge_setup.station3), checkpoint_name_text);
            sprintf(checkpoint_text, "Checkpoint 3: %s", checkpoint_name_text);
            nk_label(state.ctx, checkpoint_text, NK_TEXT_ALIGN_LEFT);
            nk_group_end(state.ctx);
        }
        nk_group_end(state.ctx);
    }
}

void challenge_c_setup_gui() {
    nk_layout_row_dynamic(state.ctx, 200, 1);
    if (nk_group_begin(state.ctx, "Challenge C", NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(state.ctx, 140, 1);
        if (nk_button_label(state.ctx, "START CHALLENGE")) {
            challenge_setup.ready_to_start = true;
        }
        nk_group_end(state.ctx);
    }
}

void challenge_progress_gui(bool* call_for_abort) {
    nk_layout_row_dynamic(state.ctx, 200, 2);
    char text[13] = "Challenge A:";
    strcpy(text, state.current_challenge == CHALLENGE_B ? "Challenge B:" : (state.current_challenge == CHALLENGE_C ? "Challenge C:" : text));
    if (nk_group_begin(state.ctx, text, NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(state.ctx, 30, 1);
        char path_description[50];
        char point1_string[10];
        char point2_string[10];
        get_lee_name(state.robot_state.current_path.points[0], point1_string);
        get_lee_name(state.robot_state.current_path.points[state.robot_state.current_path.length - 1], point2_string);
        sprintf(path_description, "Going from %s to %s.", point1_string, point2_string);
        nk_label_colored(state.ctx, path_description, NK_TEXT_ALIGN_LEFT, nk_white);
        if (state.robot_state.mines_count == 0) {
            nk_label_colored(state.ctx, "No mines detected thus far.", NK_TEXT_ALIGN_LEFT, nk_white);
        }
        else {
            char mine_string[50];
            sprintf(mine_string, "%d mines detected", state.robot_state.mines_count);
            nk_label_colored(state.ctx, mine_string, NK_TEXT_ALIGN_LEFT, nk_white);
        }

        if (state.robot_state.data_received) {
            nk_label_colored(state.ctx, "Signal Received", NK_TEXT_ALIGN_LEFT, nk_green);
        }
        else {
            nk_label_colored(state.ctx, "No Signal", NK_TEXT_ALIGN_LEFT, nk_red);
        }

        if (state.current_challenge == CHALLENGE_C && challenge_c_waiting) {
            nk_layout_row_dynamic(state.ctx, 30, 2);
            nk_label_colored(state.ctx, "Please put the robot at S1.", NK_TEXT_ALIGN_LEFT, nk_rgb(60, 180, 180));
            if (nk_button_label(state.ctx, "CONTINUE CHALLENGE C")) {
                challenge_setup.continue_challenge_c = true;
            }
        }
        else {
            nk_layout_row_dynamic(state.ctx, 30, 2);
            if (nk_button_label(state.ctx, "RESTART CHALLENGE")) {
                if (call_for_abort != NULL) {
                    *call_for_abort = true;
                }
                challenge_setup.ready_to_start = true;
                struct ChallengeSetup setup = challenge_setup;
                switch (state.current_challenge) {
                    case CHALLENGE_A:
                        stop_challenge_a(false);
                        break;
                    case CHALLENGE_B:
                        stop_challenge_b(false);
                        break;
                    case CHALLENGE_C:
                        stop_challenge_c(false);
                        break;
                }
                challenge_setup = setup;
            }
            if (nk_button_label(state.ctx, "FORCE STOP CHALLENGE")) {
                if (call_for_abort != NULL) {
                    *call_for_abort = true;
                }
                switch (state.current_challenge) {
                    case CHALLENGE_A:
                        stop_challenge_a(false);
                        break;
                    case CHALLENGE_B:
                        stop_challenge_b(false);
                        break;
                    case CHALLENGE_C:
                        stop_challenge_c(false);
                        break;
                }
            }
        }

        nk_group_end(state.ctx);
    }
    if (nk_group_begin(state.ctx, "Robot Status:", NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(state.ctx, 30, 1);
        nk_label_colored(state.ctx, "Challenge in progress...", NK_TEXT_ALIGN_LEFT, nk_green);

        char* next_instruction_string;
        switch (state.robot_state.next_instruction) {
            case FORWARD:
                next_instruction_string = "FORWARD";
                break;
            case LEFT:
                next_instruction_string = "LEFT";
                break;
            case RIGHT:
                next_instruction_string = "RIGHT";
                break;
            case STOP:
                next_instruction_string = "STOP";
                break;
        }

        nk_layout_row_dynamic(state.ctx, 30, 2);
        nk_label_colored(state.ctx, "Next Instruction: ", NK_TEXT_ALIGN_LEFT, nk_white);
        nk_label_colored(state.ctx, next_instruction_string, NK_TEXT_ALIGN_LEFT, nk_white);

        if (!state.robot_state.major_failure) {
            nk_label_colored(state.ctx, "All Systems OK", NK_TEXT_ALIGN_LEFT, nk_green);
        }
        else {
            nk_label_colored(state.ctx, "MAJOR FAILURE", NK_TEXT_ALIGN_LEFT, nk_red);
        }
        char* next_facing_string;
        switch (state.robot_state.facing) {
            case EAST:
                next_facing_string = "EAST";
                break;
            case NORTH:
                next_facing_string = "NORTH";
                break;
            case SOUTH:
                next_facing_string = "SOUTH";
                break;
            case WEST:
                next_facing_string = "WEST";
                break;
        }
        nk_layout_row_dynamic(state.ctx, 30, 2);
        nk_label_colored(state.ctx, "Currently Facing: ", NK_TEXT_ALIGN_LEFT, nk_white);
        nk_label_colored(state.ctx, next_facing_string, NK_TEXT_ALIGN_LEFT, nk_white);

        nk_group_end(state.ctx);
    }
}

void grid_gui(struct Point *selected_point, bool *grid_selected) {
    int grid_width = 600;
    int grid_height = 620;
    float ratio[3];
    ratio[0] = ratio[2] = (float) (state.window_width - grid_width) / 2;
    ratio[1] = (float) grid_width;
    nk_layout_row(state.ctx, NK_STATIC, (float) grid_height, 3, ratio);
    nk_spacing(state.ctx, 1);
    if (nk_group_begin(state.ctx, "grid", 0)) {
        *selected_point = draw_grid(state.ctx, lee_path_to_index(state.robot_state.current_path), state.robot_state.mines,
                  state.robot_state.mines_count, grid_selected, state.robot_state.last_reported_position, state.robot_state.treasures, state.robot_state.treasure_count);
        nk_group_end(state.ctx);
    }
}

struct RobotState *get_robot_state_callback() {
    return &state.robot_state;
}

void challenge_ended_callback() {
    state.challenge_state = IDLE;
    reset_challenge_setup();
}

void reset_challenge_setup() {
    challenge_setup.ready_to_start = false;
    challenge_setup.station1.x = -1;
    challenge_setup.station1.y = -1;
    challenge_setup.station2.x = -1;
    challenge_setup.station2.y = -1;
    challenge_setup.station3.x = -1;
    challenge_setup.station3.y = -1;
}