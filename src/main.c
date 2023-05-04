#include <stdio.h>
#include <sysinfoapi.h>

#include "backend/lee.h"

int main() {
    reset_lee_maze();
    //lee_add_mine(&(struct Point){.x = 5, .y = 2});
    //lee_add_mine(&(struct Point){.x = 5, .y = 10});
    //lee_add_mine(&(struct Point){.x = 10, .y = 5});
    long long start = GetTickCount();
    struct PathList pathList = lee(10, 10, 2, 2);
    long long end = GetTickCount();

    printf("Time: %lld\n", end - start);
    printf("PathList found: %d\n", pathList.length);
    struct Path selectedPath = select_path(&pathList);
    printf("Selected pathList (%d Turns): ", selectedPath.turns);
    for(int i = 0; i < selectedPath.length; i++) {
        printf("(%d, %d), ", selectedPath.points[i].x, selectedPath.points[i].y);
    }
    printf("\n\n");
    print_path(&selectedPath);
    return 0;
}