#include "challenge_a.h"

#include "../common/path.h"
#ifdef FAKE_UART
#include "../uart/fake_uart_handler.h"
#else
#include "../uart/uartHandler.h"
#endif
#include "challenge_signals.h"
#include "../mazeRouter.h"

struct Point station1;
struct Point station2;
struct Point station3;
int current_step;
bool active = false;

void path_ended(enum PathExecutionResult result);

void start_challenge_a(struct Point _station1, struct Point _station2, struct Point _station3) {
    station1 = _station1;
    station2 = _station2;
    station3 = _station3;
    initUART();

    get_robot_state()->major_failure = false;

    current_step = 0;
    active = true;

    init_maze_router();

    struct Path path = calculate_route(index_to_lee(create_point(-1, 1)), index_to_lee(station1));
    executePath(path, path_ended);
}

void path_ended(enum PathExecutionResult result) {
    if (!active) {
        return;
    }

    if (result != SUCCESS) {
        closeConnection();
        active = false;
        get_robot_state()->major_failure = true;
        challenge_ended();
        return;
    }

    switch (current_step) {
        case 0: {
            current_step = 1;
            struct Path path = calculate_route(index_to_lee(station1), index_to_lee(station2));
            executePath(path, path_ended);
            break;
        }
        case 1: {
            current_step = 2;
            struct Path path = calculate_route(index_to_lee(station2), index_to_lee(station3));
            executePath(path, path_ended);
            break;
        }
        case 2: {
            current_step = 3;
            closeConnection();
            active = false;
            get_robot_state()->major_failure = false;
            challenge_ended();
            break;
        }

        default: {
            closeConnection();
            get_robot_state()->major_failure = true;
            challenge_ended();
            break;
        }
    }
}