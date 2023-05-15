//
// Created by Jens on 5/3/2023.
//
#include <stdlib.h>
#include "point.h"

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

bool are_points_adjacent(struct Point point1, struct Point point2) {
    return ((abs(point1.x - point2.x) == 1 && point1.y == point2.y) ||
            (abs(point1.y - point2.y) == 1 && point1.x == point2.x));
}

bool is_point_equal(struct Point point1, struct Point point2) {
    return (point1.x == point2.x && point1.y == point2.y);
}