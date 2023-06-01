//
// Created by Jens on 5/3/2023.
//
#include <stdlib.h>
#include "point.h"

const struct Point crossings[5][5] = {
        {{2,2}, {4, 2}, {6, 2}, {8, 2}, {10, 2}},
        {{2,4}, {4, 4}, {6, 4}, {8, 4}, {10, 4}},
        {{2,6}, {4, 6}, {6, 6}, {8, 6}, {10, 6}},
        {{2,8}, {4, 8}, {6, 8}, {8, 8}, {10, 8}},
        {{2,10}, {4, 10}, {6, 10}, {8, 10}, {10, 10}}
};

const struct Point stations[12] = {
        {12, 4}, {12, 6}, {12, 8},
        {8, 12}, {6, 12}, {4, 12},
        {0, 8}, {0, 6}, {0, 4},
        {4, 0}, {6, 0}, {8, 0}
};

bool point_is_valid(struct Point *point) {
    return (point->x >= 0 && point->x < 13 && point->y >= 0 && point->y < 13);
}

bool is_point_connection_equal(struct PointConnection connection1, struct PointConnection connection2) {
    return ((connection1.point1.x == connection2.point1.x &&
             connection1.point1.y == connection2.point1.y &&
             connection1.point2.x == connection2.point2.x &&
             connection1.point2.y == connection2.point2.y) ||
            (connection1.point2.x == connection2.point1.x &&
             connection1.point2.y == connection2.point1.y &&
             connection1.point1.x == connection2.point2.x &&
             connection1.point1.y == connection2.point2.y));
}

bool is_connection_in_array(struct PointConnection connection, struct PointConnection* array, int connection_count) {
    for (int i = 0; i < connection_count; i++) {
        if (is_point_connection_equal(connection, array[i])) {
            return true;
        }
    }
    return false;
}

bool is_point_in_connection_array(struct Point point, struct PointConnection *array, int connection_count) {
    for (int i = 0; i < connection_count; i++) {
        if (is_point_equal(point, array[i].point1) || is_point_equal(point, array[i].point2)) {
            return true;
        }
    }
    return false;
}

bool is_point_in_connection(struct Point point, struct PointConnection connection) {
    return is_point_equal(point, connection.point1) || is_point_equal(point, connection.point2);
}

bool are_points_adjacent(struct Point point1, struct Point point2) {
    return ((abs(point1.x - point2.x) == 1 && point1.y == point2.y) ||
            (abs(point1.y - point2.y) == 1 && point1.x == point2.x));
}

bool is_point_equal(struct Point point1, struct Point point2) {
    return (point1.x == point2.x && point1.y == point2.y);
}

struct Point index_to_crossing(int x, int y) {
    return crossings[x][y];
}

struct Point index_to_station(int index) {
    return stations[index - 1];
}

struct Point crossing_to_index(struct Point point) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_point_equal(crossings[i][j], point)) {
                struct Point value;
                value.x = i;
                value.y = j;
                return value;
            }
        }
    }
    struct Point nothing;
    nothing.x = -1;
    nothing.y = -1;
    return nothing;
}

int station_to_index(struct Point point) {
    for (int i = 0;  i < 12; i++) {
        if (is_point_equal(stations[i], point)) {
            return i + 1;
        }
    }
    return -1;
}

struct Point index_to_lee(struct Point point) {
    struct Point result;
    if (point.x == -1 && point.y >= 1 && point.y <= 12) {
        result = index_to_station(point.y);
        return result;
    }

    if (point_is_valid(&point)) {
        result = index_to_crossing(point.x, point.y);
        return result;
    }

    result.x = -1;
    result.y = -1;
    return result;
}
