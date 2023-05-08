#include <stdio.h>

#include "backend/algorithms/lee.h"
#include "frontend/cli.h"
#include "backend/mazeRouter.h"

int main() {
    reset_lee_maze();
    //lee_add_mine(&(struct Point){.x = 5, .y = 2});
    //lee_add_mine(&(struct Point){.x = 5, .y = 10});
    //lee_add_mine(&(struct Point){.x = 10, .y = 5});
    struct PathList pathList = lee((struct Point){.x = 10, .y = 10}, (struct Point){.x = 2, .y = 2});

    printf("PathList found: %d\n", pathList.length);
    struct Path selectedPath = select_path(&pathList);
    printf("Selected pathList (%d Turns): ", selectedPath.turns);
    for(int i = 0; i < selectedPath.length; i++) {
        printf("(%d, %d), ", selectedPath.points[i].x, selectedPath.points[i].y);
    }
    printf("\n\n");

    init_CLI();
    return 0;
}