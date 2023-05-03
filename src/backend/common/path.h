//
// Created by Jens on 5/3/2023.
//

#ifndef SRC_PATH_H
#define SRC_PATH_H

#include "point.h"

#define MAX_PATH_LENGTH 100

struct Path {
    struct Point path[20];
    int length;
    int turns;
};

struct Paths {
    struct Path path[MAX_PATH_LENGTH];
    int length;
};

int calc_turns(struct Path *path);
struct Path select_path(struct Paths *paths);
void print_path(struct Path *path);

#endif //SRC_PATH_H
