//
// Created by Jens on 5/15/2023.
//

#ifndef SRC_UARTINSTRUCTIONS_H
#define SRC_UARTINSTRUCTIONS_H

#include "../common/point.h"

enum Instruction {
    FORWARD = 0,
    LEFT = 1,
    RIGHT = 2,
    STOP = 4
};

enum Facing {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

struct UARTInstruction {
    char instruction;
    struct Point point;
    enum Facing facing;
};

enum PathExecutionResult {
    SUCCESS,
    FAILURE,
    MINE
};

#endif //SRC_UARTINSTRUCTIONS_H
