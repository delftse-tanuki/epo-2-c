//
// Created by Jens on 4/24/2023.
//

#ifndef SRC_LEE_H
#define SRC_LEE_H

#include "../common/path.h"

struct PathList lee(struct Point source, struct Point target);
void reset_lee_maze();
void lee_add_mine(struct Point *point);

#endif //SRC_LEE_H
