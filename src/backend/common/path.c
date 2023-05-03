//
// Created by Jens on 5/3/2023.
//
#include <stdio.h>
#include <string.h>
#include "path.h"
#include "maze.h"

int calc_turns(struct Path *path) {
    int turns = 0;
    for(int i = 0; i < path->length - 2; i++) {
        int x1 = path->path[i].x;
        int y1 = path->path[i].y;
        int x2 = path->path[i + 1].x;
        int y2 = path->path[i + 1].y;
        int x3 = path->path[i + 2].x;
        int y3 = path->path[i + 2].y;
        if(x1 == x2 && x2 == x3) {
            continue;
        }
        if(y1 == y2 && y2 == y3) {
            continue;
        }
        turns++;
    }
    return turns;
}

struct Path select_path(struct Paths *paths) {
    struct Path best_path = paths->path[0];
    for(int i = 1; i < paths->length; i++) {
        if(paths->path[i].turns < best_path.turns) {
            best_path = paths->path[i];
        }
    }
    return best_path;
}

void print_path(struct Path *path) {
    int path_maze[MAZE_WIDTH][MAZE_HEIGHT];
    memcpy(path_maze, maze, sizeof(maze));
    for(int i = 0; i < path->length; i++) {
        path_maze[path->path[i].x][path->path[i].y] = 1;
    }
    for(int row = 0; row < MAZE_WIDTH; row++) {
        for(int col = 0; col < MAZE_HEIGHT; col++) {
            printf("%c", path_maze[col][row] == -1 ? 'o' : path_maze[col][row] == 0 ? ' ' : '*');
        }
        printf("\n");
    }
}