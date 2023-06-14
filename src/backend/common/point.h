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
bool is_point_in_connection_array(struct Point point, struct PointConnection* array, int connection_count);
bool is_point_in_connection(struct Point point, struct PointConnection connection);
bool are_points_adjacent(struct Point point1, struct Point point2);
struct Point index_to_crossing(int x, int y);
struct Point index_to_station(int index);
int station_to_index(struct Point point);
struct Point crossing_to_index(struct Point point);
struct Point index_to_lee(struct Point point);
struct Point lee_to_index(struct Point point);
struct Point create_point(int x, int y);
struct PointConnection create_point_connection(struct Point point1, struct Point point2);
struct PointConnection create_point_connection_shorthand(int x1, int y1, int x2, int y2);
void get_lee_name(struct Point point, char* result);

#endif //SRC_POINT_H
