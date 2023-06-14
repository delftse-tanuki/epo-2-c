#include "challenge_c.h"

#include "../common/path.h"
#ifdef FAKE_UART
#include "../uart/fake_uart_handler.h"
#else
#include "../uart/uartHandler.h"
#endif
#include "challenge_signals.h"
#include "../mazeRouter.h"

#ifdef UNIX
#include <time.h>
#endif

enum CState {
    EXPLORE,
    CLEANUP
};

static bool active = false;
struct Point major_goal;
struct Point intermittent_goal;
#define MAX_VISITED_CONNECTIONS 300
struct PointConnection visited_connections[MAX_VISITED_CONNECTIONS];
struct PointConnection cleanup_connections[MAX_VISITED_CONNECTIONS];
int visited_connection_count;
int cleanup_connection_count;
int current_cleanup_connection;
int current_cleanup_connection_point;
enum CState c_state;
bool find_treasures;
bool challenge_c_waiting;

static void path_ended(enum PathExecutionResult result);
static void sync_mazerouter_mines();
static void robot_moved(struct PointConnection movement);
void initialize_cleanup_connections();
void remove_cleanup_duplicants();

void start_challenge_c() {
    initUART();

    get_robot_state()->mines_count = 0;
    get_robot_state()->major_failure = false;
    get_robot_state()->ignore_mines = false;
    active = true;

    sync_mazerouter_mines();
    initialize_cleanup_connections();

    visited_connection_count = 0;
    current_cleanup_connection = -1;
    current_cleanup_connection_point = 1;
    c_state = EXPLORE;
    find_treasures = false;
    challenge_c_waiting = false;
    get_robot_state()->treasure_count = 0;

    major_goal = create_point(0,4);
    intermittent_goal = major_goal;
    struct Path path = calculate_route(index_to_lee(create_point(-1, 1)), index_to_lee(major_goal));
    executePath(path, 0, path_ended, robot_moved);
}

static void explore(enum PathExecutionResult result) {
    if (result == FAILURE) {
        stop_challenge_c(true);
        return;
    }

    if (result == MINE) {
        sync_mazerouter_mines();
        if (find_treasures) {
            get_robot_state()->treasures[get_robot_state()->treasure_count++] = get_robot_state()->mines[get_robot_state()->mines_count - 1];
            if (get_robot_state()->treasure_count == 3) {
                stop_challenge_c(false);
            }
#ifdef UNIX
#ifdef FAKE_UART
            struct timespec ts;
            ts.tv_sec = 2;
            ts.tv_nsec = 0;
            nanosleep(&ts, NULL);
#endif
#endif
        }
        struct Path path = calculate_route(index_to_lee(get_robot_state()->last_reported_position), index_to_lee(intermittent_goal));
        executePath(path, 0, path_ended, robot_moved);
        return;
    }

    if (is_point_equal(get_robot_state()->last_reported_position, major_goal)) {
        // Pick a new major goal.
        bool go_right = false;
        if ((major_goal.y == 0 && major_goal.x % 2 == 0) || (major_goal.y == 4 && major_goal.x % 2 != 0)) {
            go_right = true;
        }
        if (go_right) {
            major_goal.x += 1;
            if (major_goal.x > 4) {
                remove_cleanup_duplicants();
                c_state = CLEANUP;
                path_ended(SUCCESS);
                return;
            }
        }
        else {
            major_goal.y = major_goal.y == 0 ? 4 : 0;
        }
    }

    if (is_point_equal(get_robot_state()->last_reported_position, intermittent_goal)) {
        struct Path major_path = calculate_route(index_to_lee(get_robot_state()->last_reported_position), index_to_lee(major_goal));
        intermittent_goal = lee_to_index(major_path.points[1]);
    }

    struct Path path = calculate_route(index_to_lee(get_robot_state()->last_reported_position), index_to_lee(intermittent_goal));
    executePath(path, 0, path_ended, robot_moved);
}

static void cleanup(enum PathExecutionResult result) {
    // Failures are expected (I think?) so we skip checking for those.
    if (result == MINE) {
        sync_mazerouter_mines();
        if (find_treasures) {
            get_robot_state()->treasures[get_robot_state()->treasure_count++] = get_robot_state()->mines[get_robot_state()->mines_count - 1];
            if (get_robot_state()->treasure_count == 3) {
                stop_challenge_c(false);
            }

#ifdef UNIX
#ifdef FAKE_UART
            struct timespec ts;
            ts.tv_sec = 2;
            ts.tv_nsec = 0;
            nanosleep(&ts, NULL);
#endif
#endif
        }
    }

    if (current_cleanup_connection_point == 0) {
        current_cleanup_connection_point = 1;
    }
    else {
        current_cleanup_connection++;
        current_cleanup_connection_point = 0;
    }

    if (current_cleanup_connection >= cleanup_connection_count) {
        if (find_treasures) {
            // We shouldn't ever have this happen - We should have found 3 treasures before we've visited everything.
            stop_challenge_c(true);
            return;
        }
        // Wait for continue_challenge_c().
        challenge_c_waiting = true;
        return;
    }

    struct Point goal = current_cleanup_connection_point == 0 ? cleanup_connections[current_cleanup_connection].point1 : cleanup_connections[current_cleanup_connection].point2;
    struct Path path = calculate_route(index_to_lee(get_robot_state()->last_reported_position), index_to_lee(goal));
    executePath(path, 0, path_ended, robot_moved);
}

static void path_ended(enum PathExecutionResult result) {
    if (!active) {
        return;
    }

    switch (c_state) {
        case EXPLORE:
            explore(result);
            break;
        case CLEANUP:
            cleanup(result);
            break;
    }
}

static void sync_mazerouter_mines() {
    init_maze_router();

    for (int i = 0; i < get_robot_state()->mines_count; i++) {
        struct PointConnection lee_mine;
        lee_mine.point1 = index_to_lee(get_robot_state()->mines[i].point1);
        lee_mine.point2 = index_to_lee(get_robot_state()->mines[i].point2);
        add_point_connection_mine(&lee_mine);
    }
}

void robot_moved(struct PointConnection movement) {
    if (!active) {
        return;
    }

    visited_connections[visited_connection_count++] = movement;
}

void initialize_cleanup_connections() {
    cleanup_connections[0] = create_point_connection_shorthand(4, 0, 3, 0);
    cleanup_connections[1] = create_point_connection_shorthand(2, 0, 1, 0);
    cleanup_connections[2] = create_point_connection_shorthand(0, 1, 1, 1);
    cleanup_connections[3] = create_point_connection_shorthand(1, 1, 2, 1);
    cleanup_connections[4] = create_point_connection_shorthand(2, 1, 3, 1);
    cleanup_connections[5] = create_point_connection_shorthand(3, 1, 4, 1);
    cleanup_connections[6] = create_point_connection_shorthand(3, 2, 4, 2);
    cleanup_connections[7] = create_point_connection_shorthand(2, 2, 3, 2);
    cleanup_connections[8] = create_point_connection_shorthand(1, 2, 2, 2);
    cleanup_connections[9] = create_point_connection_shorthand(0, 2, 1, 2);
    cleanup_connections[10] = create_point_connection_shorthand(0, 3, 1, 3);
    cleanup_connections[11] = create_point_connection_shorthand(1, 3, 2, 3);
    cleanup_connections[12] = create_point_connection_shorthand(2, 3, 3, 3);
    cleanup_connections[13] = create_point_connection_shorthand(3, 3, 4, 3);
    cleanup_connections[14] = create_point_connection_shorthand(2, 4, 3, 4);
    cleanup_connection_count = 15;
}

void remove_cleanup_duplicants() {
    struct PointConnection new_cleanup_connections[MAX_VISITED_CONNECTIONS];
    int new_count = 0;

    for (int i = 0; i < cleanup_connection_count; i++) {
        if (!is_connection_in_array(cleanup_connections[i], visited_connections, visited_connection_count) &&
            !is_connection_in_array(cleanup_connections[i], get_robot_state()->mines, get_robot_state()->mines_count)) {
            new_cleanup_connections[new_count++] = cleanup_connections[i];
        }
    }

    for (int i = 0; i < new_count; i++) {
        cleanup_connections[i] = new_cleanup_connections[i];
    }

    cleanup_connection_count = new_count;
}

void continue_challenge_c() {
    find_treasures = true;
    c_state = EXPLORE;
    challenge_c_waiting = false;

    initialize_cleanup_connections();

    visited_connection_count = 0;
    current_cleanup_connection = -1;
    current_cleanup_connection_point = 1;

#ifdef FAKE_UART
    add_treasures = true;
#endif

    major_goal = create_point(0,4);
    intermittent_goal = major_goal;
    struct Path path = calculate_route(index_to_lee(create_point(-1, 1)), index_to_lee(major_goal));
    executePath(path, 0, path_ended, robot_moved);
}

void stop_challenge_c(bool failure) {
    closeConnection();
    active = false;
    get_robot_state()->major_failure = failure;
    challenge_ended();
}
