//
// Created by Jens on 5/8/2023.
//

#ifndef SRC_MAZEROUTER_H
#define SRC_MAZEROUTER_H

#include "common/path.h"
#include "common/point.h"

void init_maze_router();
void add_point_connection_mine(struct PointConnection *connection);
void add_mine(struct Point *point);
struct Path calculate_route(struct Point source, struct Point destination);
struct Path select_path(struct PathList *paths);

#endif //SRC_MAZEROUTER_H
