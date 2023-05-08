//
// Created by Jens on 5/3/2023.
//

#ifndef SRC_PATH_H
#define SRC_PATH_H

#include "point.h"

#define MAX_PATH_AMOUNT 100

struct Path {
    struct Point points[20];
    int length;
    int turns;
};

struct PathList {
    struct Path path[MAX_PATH_AMOUNT];
    int length;
};

int calc_turns(struct Path *path);

#endif //SRC_PATH_H
