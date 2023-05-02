#include <stdio.h>
#include <sysinfoapi.h>

#include "backend/lee.h"

int main() {
    for(int row = 0; row < MAZE_WIDTH; row++) {
        for(int col = 0; col < MAZE_HEIGHT; col++) {
            printf("%d,\t", maze[col][row]);
        }
        printf("\n");
    }
    long long start = GetTickCount();
    struct Paths path = lee(10, 10, 10, 2);
    long long end = GetTickCount();
    printf("\n");
    for(int row = 0; row < MAZE_WIDTH; row++) {
        for(int col = 0; col < MAZE_HEIGHT; col++) {
            printf("%d,\t", maze[col][row]);
        }
        printf("\n");
    }

    printf("\n\nTime: %lld", end - start);
    printf("\nPaths found: %d", path.length);
    for(int i = 0; i < path.length; i++) {
        printf("\nPath %d (Turns %d): ", i + 1, calc_turns(&path.path[i]));
        for(int j = 0; j < path.path[i].length; j++) {
            printf("(%d, %d), ", path.path[i].path[j].x, path.path[i].path[j].y);
        }
    }
    int fake_map[13][13] = {
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
    for(int i = 0; i < path.path[5].length; i++) {
        fake_map[path.path[5].path[i].x][path.path[5].path[i].y] = 1;
    }
    printf("\n\n");
    for(int row = 0; row < MAZE_WIDTH; row++) {
        for(int col = 0; col < MAZE_HEIGHT; col++) {
            printf("%d,\t", fake_map[col][row]);
        }
        printf("\n");
    }
    return 0;
}
