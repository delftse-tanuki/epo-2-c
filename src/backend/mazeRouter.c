//
// Created by Jens on 5/8/2023.
//
#include "mazeRouter.h"
#include "algorithms/lee.h"

struct Mines {
    struct Point mines[15];
    int length;
};

const struct Point crossings[5][5] = {
        {{2,2}, {4, 2}, {6, 2}, {8, 2}, {10, 2}},
        {{2,4}, {4, 4}, {6, 4}, {8, 4}, {10, 4}},
        {{2,6}, {4, 6}, {6, 6}, {8, 6}, {10, 6}},
        {{2,8}, {4, 8}, {6, 8}, {8, 8}, {10, 8}},
        {{2,10}, {4, 10}, {6, 10}, {8, 10}, {10, 10}}
};

const struct Point stations[12] = {
        {10, 4}, {10, 6}, {10, 8},
        {8, 12}, {6, 12}, {4, 12},
        {2, 8}, {2, 6}, {2, 4},
        {4, 0}, {6, 0}, {8, 0}
};

struct Mines mines;

void init_maze_router() {
    mines.length = 0;
}

struct PathList calculate_route(struct Point *source, struct Point *destination) {
    reset_lee_maze();
    for(int i = 0; i < mines.length; i++) {
        lee_add_mine(&mines.mines[i]);
    }
    return lee(*source, *destination);
}

void add_mine(struct Point *point) {
    mines.mines[mines.length++] = *point;
    lee_add_mine(point);
}

struct Path select_path(struct PathList *paths) {
    struct Path best_path = paths->path[0];
    for(int i = 1; i < paths->length; i++) {
        if(paths->path[i].turns < best_path.turns) {
            best_path = paths->path[i];
        }
    }
    return best_path;
}