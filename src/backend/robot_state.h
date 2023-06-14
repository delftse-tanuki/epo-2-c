#ifndef EPO2_ROBOT_STATE_H
#define EPO2_ROBOT_STATE_H

#include "uart/UARTInstructions.h"
#include "common/path.h"

struct RobotState {
    // What way is the robot currently facing?
    enum Facing facing;
    // What's the next instruction we'll be sending to the robot?
    enum Instruction next_instruction;
    // What's the robot's last reported position? In index format.
    struct Point last_reported_position;
    // Did we receive a byte from the robot in the last process loop?
    bool data_received;
    // Did we suffer a major failure that requires a reset?
    bool major_failure;
    // The mines that the robot has detected so far.
    struct PointConnection mines[100];
    // Ignore mines?
    bool ignore_mines;
    // How many mines have we found?
    int mines_count;
    // The path the robot is currently following.
    struct Path current_path;
    // The treasures we've found so far.
    struct PointConnection treasures[3];
    // The amount of treasures we've found.
    int treasure_count;
};

#endif //EPO2_ROBOT_STATE_H
