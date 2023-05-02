//
// Created by Jens on 4/24/2023.
//

#include <stdbool.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "lee.h"

int maze[MAZE_WIDTH][MAZE_HEIGHT] = {
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
        {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},
        {-1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1},
        {-1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1},
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}
};

// Directions: 0 = North, 1 = East, 2 = South, 3 = West
int directions_x[4] = {0, 1, 0, -1};
int directions_y[4] = {1, 0, -1, 0};

int calc_turns(struct Paths *path) {
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

void trace_nodes(struct Node *current) {
    if(current->distance <= 1) {
        return;
    }
    int options = 0;
    for(int i = 0; i < 4; i++) {
        struct Point next = {current->point.x + directions_x[i], current->point.y + directions_y[i]};
        if(next.x < 0 || next.x >= MAZE_WIDTH || next.y < 0 || next.y >= MAZE_HEIGHT) {
            continue;
        }
        if(maze[next.x][next.y] == current->distance - 1) {
            struct Node *next_node = malloc(sizeof(struct Node));
            next_node->distance = maze[next.x][next.y];
            next_node->point = next;
            next_node->options = 0;
            memset(next_node->next, 0, sizeof(next_node->next));
            current->next[options++] = next_node;
        }
    }
    for(int i = 0; i < options; i++) {
        current->options = options;
        trace_nodes(current->next[i]);
    }
}

void create_paths(struct Node *current, struct Paths *paths) {
    paths->path[paths->length++] = current->point;
    for(int i = 0; i < current->options; i++) {
        struct Node *next = current->next[i];
        struct Paths *path = malloc(sizeof(struct Paths));
        path->length = paths->length;
        memcpy(path->path, paths->path, sizeof(paths->path));
        paths->next = path;
        create_paths(next, paths);
    }

}

struct Paths lee_algorithm(struct Point source, struct Point target) {
    bool visited[MAZE_WIDTH][MAZE_HEIGHT];
    for(int i = 0; i < MAZE_WIDTH; i++) {
        for(int j = 0; j < MAZE_HEIGHT; j++) {
            visited[i][j] = false;
        }
    }
    visited[target.x][target.y] = true;
    struct Node queue[169];
    int queue_start = 0;
    int queue_end = 0;
    queue[queue_end++] = (struct Node) {target, 1};
    maze[target.x][target.y] = 1;
    while(maze[source.x][source.y] == 0 && queue_start != queue_end) {
        struct Node current = queue[queue_start++];
        for(int i = 0; i < 4; i++) {
            struct Point next = {current.point.x + directions_x[i], current.point.y + directions_y[i]};
            if(next.x < 0 || next.x >= MAZE_WIDTH || next.y < 0 || next.y >= MAZE_HEIGHT) {
                continue;
            }
            if(visited[next.x][next.y] || maze[next.x][next.y] == -1) {
                continue;
            }
            visited[next.x][next.y] = true;
            queue[queue_end++] = (struct Node) {next, current.distance + 1};
            maze[next.x][next.y] = current.distance + 1;
        }
    }
    struct Paths paths = {0};
    struct Node current = {source, maze[source.x][source.y]};
    trace_nodes(&current);
    create_paths(&current, &paths);

    return paths;
}

struct Paths lee(int sourceX, int sourceY, int destinationX, int destinationY) {
    struct Point source = {sourceX, sourceY};
    struct Point dest = {destinationX, destinationY};
    return lee_algorithm(source, dest);
}