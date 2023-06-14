#ifdef UNIX
#include <time.h>
#endif
#ifdef FAKE_UART
#include "fake_uart_handler.h"

#include "stdio.h"
#include "../robot_state.h"
#include "../challenges/challenge_signals.h"
#include "../../gui/gui.h"

bool add_treasures = false;

bool fake_mine(struct Point current_point, struct Point next_point, struct PointConnection mine);

void initUART() {
    printf("FAKE UART INITIALIZED\n");
    get_robot_state()->mines_count = 0;
}

void executePath(struct Path path, __attribute__((unused)) int is_last, void (*path_ended)(enum PathExecutionResult), void (*robot_moved)(struct PointConnection movement)) {
    get_robot_state()->current_path = path;

    for (int i = 0; i < path.length; i++) {
#ifdef UNIX
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 250000000;
        nanosleep(&ts, NULL);
#endif

        if (robot_moved != NULL) {
            robot_moved(create_point_connection(get_robot_state()->last_reported_position, lee_to_index(path.points[i])));
        }
        get_robot_state()->last_reported_position = lee_to_index(path.points[i]);

        #define TEST_MINES_COUNT 12
        struct PointConnection mines[TEST_MINES_COUNT] = {
                create_point_connection(create_point(0, 0), create_point(1, 0)),
                create_point_connection(create_point(2, 0), create_point(3, 0)),
                create_point_connection(create_point(3, 0), create_point(4, 0)),
                create_point_connection(create_point(1, 1), create_point(2, 1)),
                create_point_connection(create_point(2, 1), create_point(3, 1)),
                create_point_connection(create_point(0, 2), create_point(1, 2)),
                create_point_connection(create_point(2, 2), create_point(2, 3)),
                create_point_connection(create_point(3, 2), create_point(4, 2)),
                create_point_connection(create_point(4, 2), create_point(4, 3)),
                create_point_connection(create_point(0, 3), create_point(0, 4)),
                create_point_connection(create_point(2, 3), create_point(2, 4)),
                create_point_connection(create_point(3, 3), create_point(3, 4)),
        };
        for (int j = 0; j < TEST_MINES_COUNT; j++) {
            if (fake_mine(lee_to_index(path.points[i]), lee_to_index(path.points[i + 1]), mines[j])) {
                path_ended(MINE);
                return;
            }
        }

        if (add_treasures) {
            struct PointConnection treasures[3] = {
                    create_point_connection(create_point(1, 1), create_point(1, 2)),
                    create_point_connection(create_point(1, 3), create_point(2, 3)),
                    create_point_connection(create_point(0, 1), create_point(0, 2))
            };

            for (int j = 0; j < 3; j++) {
                if (fake_mine(lee_to_index(path.points[i]), lee_to_index(path.points[i + 1]), treasures[j])) {
                    path_ended(MINE);
                    return;
                }
            }
        }

#ifdef GUI
        bool call_for_abort = false;
        gui_update(&call_for_abort);
        if (call_for_abort) {
            return;
        }
#endif
    }

    if (simulate_uart_failure) {
        path_ended(FAILURE);
    }
    else {
        path_ended(SUCCESS);
    }
}

void closeConnection() {
    printf("UART CONNECTION CLOSED\n");
}

bool fake_mine(struct Point current_point, struct Point next_point, struct PointConnection mine) {
    if (!get_robot_state()->ignore_mines && ((is_point_equal(current_point, mine.point1) && is_point_equal(next_point, mine.point2)) ||
            is_point_equal(current_point, mine.point2) && is_point_equal(next_point, mine.point1))) {
        get_robot_state()->mines[get_robot_state()->mines_count] = mine;
        get_robot_state()->mines_count++;
        return true;
    }
    return false;
}
#endif