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
    printf("\nPath length: %d", path.length);
    printf("\nPath: ");
    for(int i = 0; i < path.length; i++) {
        printf("(%d, %d) ", path.path[i].x, path.path[i].y);
    }
    printf("\nTurns: %d", calc_turns(&path));

    int path_map[MAZE_WIDTH][MAZE_HEIGHT] = {0};
    for(int i = 0; i < path.length; i++) {
        path_map[path.path[i].x][path.path[i].y] = 1;
    }
    printf("\n\n");
    for(int row = 0; row < MAZE_WIDTH; row++) {
        for(int col = 0; col < MAZE_HEIGHT; col++) {
            printf("%d,\t", path_map[col][row]);
        }
        printf("\n");
    }
    return 0;
}
