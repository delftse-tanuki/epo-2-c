//
// Created by Jens on 5/3/2023.
//

#ifndef SRC_POINT_H
#define SRC_POINT_H

#include <stdbool.h>

struct Point {
    int x;
    int y;
};

struct PointConnection {
    struct Point point1;
    struct Point point2;
};

bool point_is_valid(struct Point *point);

bool is_point_equal(struct Point point1, struct Point point2);
bool is_point_connection_equal(struct PointConnection connection1, struct PointConnection connection2);
bool is_connection_in_array(struct PointConnection connection, struct PointConnection* array, int connection_count);
bool are_points_adjacent(struct Point point1, struct Point point2);

#endif //SRC_POINT_H
