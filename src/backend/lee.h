//
// Created by Jens on 4/24/2023.
//

#ifndef SRC_LEE_H
#define SRC_LEE_H

#include "common/path.h"

struct PathList lee(int sourceX, int sourceY, int destinationX, int destinationY);
void reset_lee_maze();
void lee_add_mine(struct Point *point);

#endif //SRC_LEE_H
