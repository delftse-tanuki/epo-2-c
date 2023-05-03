//
// Created by Jens on 4/24/2023.
//

#include <stdbool.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "lee.h"

int lee_maze[MAZE_WIDTH][MAZE_HEIGHT];

// Directions: 0 = North, 1 = East, 2 = South, 3 = West
int directions_x[4] = {0, 1, 0, -1};
int directions_y[4] = {1, 0, -1, 0};

struct Node {
    struct Point point;
    int distance;
    int options;
    struct Node *next[4];
};

void trace_nodes(struct Node *current) {
    if(current->distance <= 1) {
        return;
    }
    int options = 0;
    for(int i = 0; i < 4; i++) {
        struct Point next = {current->point.x + directions_x[i], current->point.y + directions_y[i]};
        if(!point_is_valid(&next)) {
            continue;
        }
        if(lee_maze[next.x][next.y] == current->distance - 1) {
            struct Node *next_node = malloc(sizeof(struct Node));
            next_node->distance = lee_maze[next.x][next.y];
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

void initialize_paths(struct Node *current, struct Path path, struct Paths *paths) {
    if(current == NULL || paths->length == MAX_PATH_LENGTH) {
        return;
    }

    path.path[path.length++] = current->point;

    if(current->options == 0) {
        memcpy(&paths->path[paths->length], &path, sizeof(struct Path));
        paths->path[paths->length].turns = calc_turns(&path);
        paths->path[paths->length].length = path.length;
        paths->length++;
    }
    for(int i = 0; i < current->options; i++) {
        initialize_paths(current->next[i], path, paths);
    }
}

void populate_map(struct Point source, struct Point target) {
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
    lee_maze[target.x][target.y] = 1;
    while(lee_maze[source.x][source.y] == 0 && queue_start != queue_end) {
        struct Node current = queue[queue_start++];
        for(int i = 0; i < 4; i++) {
            struct Point next = {current.point.x + directions_x[i], current.point.y + directions_y[i]};
            if(!point_is_valid(&next)) {
                continue;
            }
            if(visited[next.x][next.y] || lee_maze[next.x][next.y] == -1) {
                continue;
            }
            visited[next.x][next.y] = true;
            queue[queue_end++] = (struct Node) {next, current.distance + 1};
            lee_maze[next.x][next.y] = current.distance + 1;
        }
    }
}

struct Paths calculate_paths(struct Point source) {
    struct Paths paths = {0};
    struct Path path = {0};
    struct Node current = {source, lee_maze[source.x][source.y]};
    trace_nodes(&current);
    initialize_paths(&current, path, &paths);
    return paths;
}

struct Paths lee(int sourceX, int sourceY, int destinationX, int destinationY) {
    struct Point source = {sourceX, sourceY};
    struct Point dest = {destinationX, destinationY};
    populate_map(source, dest);
    return calculate_paths(source);
}

void reset_lee_maze() {
    memcpy(lee_maze, maze, sizeof(maze));
}

void lee_add_mine(struct Point *point) {
    lee_maze[point->x][point->y] = -1;
}