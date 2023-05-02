//
// Created by Jens on 4/24/2023.
//

#ifndef SRC_LEE_H
#define SRC_LEE_H

#define MAZE_WIDTH 13
#define MAZE_HEIGHT 13

struct Point {
    int x;
    int y;
};

struct Node {
    struct Point point;
    int distance;
};

struct Path {
    struct Point points[MAZE_WIDTH * MAZE_HEIGHT];
    int length;
};

extern int maze[MAZE_WIDTH][MAZE_HEIGHT];

extern struct Path lee(int sourceX, int sourceY, int destinationX, int destinationY);

extern int calc_turns(struct Path path);

#endif //SRC_LEE_H
