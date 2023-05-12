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
        {12, 4}, {12, 6}, {12, 8},
        {8, 12}, {6, 12}, {4, 12},
        {0, 8}, {0, 6}, {0, 4},
        {4, 0}, {6, 0}, {8, 0}
};

struct Mines mines;

void init_maze_router() {
    mines.length = 0;
}

struct Point index_to_crossing(int x, int y) {
    return crossings[x][y];
}

struct Point index_to_station(int index) {
    return stations[index - 1];
}

struct Path calculate_route(struct Point source, struct Point destination) {
    reset_lee_maze();
    for(int i = 0; i < mines.length; i++) {
        lee_add_mine(&mines.mines[i]);
    }
    struct PathList pathList = lee(source, destination);
    return select_path(&pathList);
}

void add_mine(struct Point *point) {
    mines.mines[mines.length++] = *point;
    lee_add_mine(point);
}

void point_connection_mine(struct PointConnection *connection) {
    struct Point mine = connection->point1;
    if(connection->point1.x == connection->point2.x) {
        if(connection->point1.y > connection->point2.y) {
            mine.y--;
            add_mine(&mine);
        } else {
            mine.y++;
            add_mine(&mine);
        }
    } else {
        if(connection->point1.x > connection->point2.x) {
            mine.x--;
            add_mine(&mine);
        } else {
            mine.x++;
            add_mine(&mine);
        }
    }
}

struct Path select_path(struct PathList *paths) {
    struct Path best_path = paths->path[0];
    for(int i = 1; i < paths->length; i++) {
        if(paths->path[i].turns < best_path.turns) {
            best_path = paths->path[i];
        }
    }

    struct Path without_edges;
    without_edges.turns = 0;
    without_edges.length = 0;
    int index = 0;

    //remove edges
    for(int i = 0; i < best_path.length; i++) {
        if(best_path.points[i].x % 2 == 0 && best_path.points[i].y % 2 == 0) {
            without_edges.points[without_edges.length++] = best_path.points[i];
        }
    }
    without_edges.turns = calc_turns(&best_path);

    return without_edges;
}