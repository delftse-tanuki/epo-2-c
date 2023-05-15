//
// Created by Jens on 5/8/2023.
//

#ifndef SRC_MAZEROUTER_H
#define SRC_MAZEROUTER_H

#include "common/path.h"
#include "common/point.h"

void init_maze_router();
struct Point index_to_crossing(int x, int y);
struct Point index_to_station(int index);
void add_mine(struct Point *point);
struct Path calculate_route(struct Point source, struct Point destination);
struct Path select_path(struct PathList *paths);

#endif //SRC_MAZEROUTER_H
