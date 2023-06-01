//
// Created by amara on 6/1/23.
//

#ifndef EPO2_ROBOT_STATE_H
#define EPO2_ROBOT_STATE_H

#include "uart/UARTInstructions.h"

struct RobotState {
    enum Facing facing;
    enum Instruction next_instruction;
    struct PointConnection current_position;
    bool data_received;
};

#endif //EPO2_ROBOT_STATE_H
