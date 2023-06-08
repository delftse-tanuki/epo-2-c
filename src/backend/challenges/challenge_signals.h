//
// Created by amara on 6/5/23.
//

#ifndef EPO2_CHALLENGE_SIGNALS_H
#define EPO2_CHALLENGE_SIGNALS_H

#include "../robot_state.h"

enum Challenges {
    CHALLENGE_A,
    CHALLENGE_B,
    CHALLENGE_C
};

extern void (*challenge_ended)();
extern struct RobotState* (*get_robot_state)();

#endif //EPO2_CHALLENGE_SIGNALS_H
