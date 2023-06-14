#ifndef EPO2_GUI_H
#define EPO2_GUI_H

#include <stdbool.h>
#include "../backend/challenges/challenge_signals.h"
#include "../backend/robot_state.h"

enum ChallengeState {
    IDLE,
    CHALLENGE_SETUP,
    CHALLENGE_IN_PROGRESS
};

struct AppState {
    enum Challenges current_challenge;
    enum ChallengeState challenge_state;
    struct RobotState robot_state;
    struct GLFWwindow* window;
    int window_width;
    int window_height;
    struct nk_context* ctx;
};

extern struct AppState state;

void gui_initialize();
void gui_update(bool* call_for_abort);

#endif //EPO2_GUI_H
