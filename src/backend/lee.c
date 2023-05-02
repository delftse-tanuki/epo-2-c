//
// Created by Jens on 4/24/2023.
//

#include <stdbool.h>
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

int calc_turns(struct Path path) {
    int turns = 0;
for(int i = 0; i < path.length - 2; i++) {
        int x1 = path.points[i].x;
        int y1 = path.points[i].y;
        int x2 = path.points[i + 1].x;
        int y2 = path.points[i + 1].y;
        int x3 = path.points[i + 2].x;
        int y3 = path.points[i + 2].y;
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

struct Path lee_algorithm(struct Point source, struct Point target) {
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
    struct Path path;
    path.length = 0;
    struct Node current = {source, maze[source.x][source.y]};
    while(current.distance > 1) {
        for(int i = 0; i < 4; i++) {
            struct Point next = {current.point.x + directions_x[i], current.point.y + directions_y[i]};
            if(next.x < 0 || next.x >= MAZE_WIDTH || next.y < 0 || next.y >= MAZE_HEIGHT) {
                continue;
            }
            if(maze[next.x][next.y] == current.distance - 1) {
                path.points[path.length++] = current.point;
                current = (struct Node) {next, maze[next.x][next.y]};
                break;
            }
        }
    }
    path.points[path.length++] = current.point;
    return path;
}

struct Path lee(int sourceX, int sourceY, int destinationX, int destinationY) {
    struct Point source = {sourceX, sourceY};
    struct Point dest = {destinationX, destinationY};
    return lee_algorithm(source, dest);
}
/*
typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction;

int direction_mask[4] = {-13, 1, 13, -1};

int lee_map[169] = {
        -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1,
        -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1,
        -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1,
        -1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        -1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        -1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        -1, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, -1,
        -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1,
        -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1,
        -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1
};

const int crossing_mask[5][5] = {
        {28, 54, 80, 106, 132},
        {30, 56, 82, 108, 134},
        {32, 58, 84, 110, 136},
        {34, 60, 86, 112, 138},
        {36, 62, 88, 114, 140}
};

const int station_mask[12] = {
        160, 162, 164, 116, 90, 64, 8, 6, 4, 52, 78, 104
};

int direction_index(int crossingIndex, direction direction) {
    int index = crossingIndex + direction_mask[direction];
    if(index < 0 || index >= 169)
        return crossingIndex;
    return index;
}

void lee_algorithm(int sourceIndex, int destinationIndex) {
    int queue[169];
    int queue_start = 0;
    int queue_end = 0;
    queue[queue_end++] = destinationIndex;
    lee_map[destinationIndex] = 1;
    while(lee_map[sourceIndex] == 0 && queue_start < queue_end) {
        int current_index = queue[queue_start++];
        for(int direction = NORTH; direction <= WEST; direction++) {
            int neighbour_index = direction_index(current_index, direction);
            if(lee_map[neighbour_index] == 0 && neighbour_index != current_index) {
                lee_map[neighbour_index] = lee_map[current_index] + 1;
                queue[queue_end++] = neighbour_index;
            }
        }
    }
}

void lee(int sourceX, int sourceY, int destinationX, int destinationY) {
    int sourceIndex = crossing_mask[sourceX][sourceY];
    int destinationIndex = crossing_mask[destinationX][destinationY];
    lee_algorithm(sourceIndex, destinationIndex);
}
*/