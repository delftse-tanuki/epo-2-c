void (*challenge_ended)(void);
void (*update_robot_status)(struct RobotState status);
struct RobotState (*get_robot_state)();