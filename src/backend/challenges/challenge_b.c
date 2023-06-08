#include "challenge_b.h"

#include "../common/path.h"
#ifdef FAKE_UART
#include "../uart/fake_uart_handler.h"
#else
#include "../uart/uartHandler.h"
#endif
#include "challenge_signals.h"
#include "../mazeRouter.h"

#include "../../gui/gui.h"

static struct Point stations[3];
static int current_step;
static bool active = false;

static void path_ended(enum PathExecutionResult result);
void sync_mazerouter_mines();

void start_challenge_b(struct Point _station1, struct Point _station2, struct Point _station3) {
    struct Point stations_temp[3];
    stations_temp[0] = _station1;
    stations_temp[1] = _station2;
    stations_temp[2] = _station3;
    initUART();

    get_robot_state()->mines_count = 0;
    get_robot_state()->major_failure = false;
    get_robot_state()->ignore_mines = false;
    current_step = 0;
    active = true;

    sync_mazerouter_mines();


    int possible_orders[6][3];
    struct Path possible_paths[6];

    int cpath = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            possible_orders[cpath][0] = i;
            switch (i) {
                case 0: {
                    possible_orders[cpath][1] = 1 + j;
                    possible_orders[cpath][2] = 2 - j;
                    break;
                }
                case 1: {
                    possible_orders[cpath][1] = j == 0 ? 0 : 2;
                    possible_orders[cpath][2] = j == 0 ? 2 : 0;
                    break;
                }
                case 2: {
                    possible_orders[cpath][1] = 1 - j;
                    possible_orders[cpath][2] = 0 + j;
                }
                default: break;
            }
            struct Path path1 = calculate_route(index_to_lee(create_point(-1, 1)), index_to_lee(stations_temp[possible_orders[cpath][0]]));
            struct Path path2 = calculate_route(index_to_lee(stations_temp[possible_orders[cpath][0]]), index_to_lee(stations_temp[possible_orders[cpath][1]]));
            struct Path path3 = calculate_route(index_to_lee(stations_temp[possible_orders[cpath][1]]), index_to_lee(stations_temp[possible_orders[cpath][2]]));
            possible_paths[cpath] = connect_paths(path1, connect_paths(path2, path3));
            cpath++;
        }
    }

    int shortest_path = 0;
    for (int i = 0; i < 6; i++) {
        if (possible_paths[i].turns < possible_paths[shortest_path].turns) {
            shortest_path = i;
        }
        else if (possible_paths[i].turns == possible_paths[shortest_path].turns) {
            if (possible_paths[i].length < possible_paths[shortest_path].length) {
                shortest_path = i;
            }
        }
    }

    stations[0] = stations_temp[possible_orders[shortest_path][0]];
    stations[1] = stations_temp[possible_orders[shortest_path][1]];
    stations[2] = stations_temp[possible_orders[shortest_path][2]];

    struct Path path = calculate_route(index_to_lee(create_point(-1, 1)), index_to_lee(stations[0]));
    executePath(path, path_ended);
}

static void path_ended(enum PathExecutionResult result) {
    if (!active) {
        return;
    }

    if (result == FAILURE) {
        closeConnection();
        active = false;
        get_robot_state()->major_failure = true;
        challenge_ended();
        return;
    }

    if (result == MINE) {
        sync_mazerouter_mines();
        struct Path path = calculate_route(index_to_lee(get_robot_state()->last_reported_position), index_to_lee(current_step == 0 ? stations[0] : (current_step == 1 ? stations[1] : stations[2])));
        executePath(path, path_ended);
        return;
    }

    switch (current_step) {
        case 0: {
            current_step = 1;
            struct Path path = calculate_route(index_to_lee(stations[0]), index_to_lee(stations[1]));
            executePath(path, path_ended);
            break;
        }
        case 1: {
            current_step = 2;
            struct Path path = calculate_route(index_to_lee(stations[1]), index_to_lee(stations[2]));
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

void sync_mazerouter_mines() {
    init_maze_router();

    for (int i = 0; i < get_robot_state()->mines_count; i++) {
        struct PointConnection lee_mine;
        lee_mine.point1 = index_to_lee(get_robot_state()->mines[i].point1);
        lee_mine.point2 = index_to_lee(get_robot_state()->mines[i].point2);
        add_point_connection_mine(&lee_mine);
    }
}