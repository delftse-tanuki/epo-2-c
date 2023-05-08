//
// Created by amara on 4/24/23.
//

#ifndef SRC_ASTAR_H
#define SRC_ASTAR_H

#include "common/point.h"
#include "common/path.h"

extern const int ASTAR_GRID_SIZE;

struct PointListEntry {
    struct Point point;
    struct PointListEntry *next;
};

struct Path get_path(struct Point start, struct Point goal,
                                struct PointConnection mines[ASTAR_GRID_SIZE * ASTAR_GRID_SIZE], int mine_count);

#endif //SRC_ASTAR_H
