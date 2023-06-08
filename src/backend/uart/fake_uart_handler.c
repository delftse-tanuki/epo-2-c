#ifdef UNIX
#include <time.h>
#endif
#include "fake_uart_handler.h"

#include "stdio.h"
#include "../robot_state.h"
#include "../challenges/challenge_signals.h"
#include "../../gui/gui.h"

void initUART() {
    printf("FAKE UART INITIALIZED\n");
    get_robot_state()->mines_count = 0;
}

void executePath(struct Path path, void (*path_ended)(enum PathExecutionResult)) {
    get_robot_state()->current_path = path;

    for (int i = 0; i < path.length; i++) {
#ifdef UNIX
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 500000000;
        nanosleep(&ts, NULL);
#endif
        get_robot_state()->last_reported_position = lee_to_index(path.points[i]);
        if (!get_robot_state()->ignore_mines && is_point_equal(lee_to_index(path.points[i]), create_point(1, 2)) && is_point_equal(lee_to_index(path.points[i + 1]), create_point(1, 1))) {
            struct PointConnection connection;
            connection.point1 = create_point(1, 1);
            connection.point2 = create_point(1, 2);
            get_robot_state()->mines[0] = connection;
            get_robot_state()->mines_count = 1;

            path_ended(MINE);
            return;
        }
        if (!get_robot_state()->ignore_mines && is_point_equal(lee_to_index(path.points[i]), create_point(3, 2)) && is_point_equal(lee_to_index(path.points[i + 1]), create_point(4, 2))) {
            struct PointConnection connection;
            connection.point1 = create_point(3, 2);
            connection.point2 = create_point(4, 2);
            get_robot_state()->mines[0] = connection;
            get_robot_state()->mines_count = 1;

            path_ended(MINE);
            return;
        }
#ifdef GUI
        gui_update();
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