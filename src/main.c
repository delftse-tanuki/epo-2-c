#include <stdio.h>
#include <sysinfoapi.h>

#include "backend/lee.h"

int main() {
    reset_lee_maze();
    lee_add_mine(&(struct Point){.x = 5, .y = 2});
    lee_add_mine(&(struct Point){.x = 5, .y = 10});
    lee_add_mine(&(struct Point){.x = 10, .y = 5});
    long long start = GetTickCount();
    struct Paths path = lee(10, 10, 2, 2);
    long long end = GetTickCount();

    printf("Time: %lld\n", end - start);
    printf("Paths found: %d\n", path.length);
    struct Path selected = select_path(&path);
    printf("Selected path (%d Turns): ", selected.turns);
    for(int i = 0; i < selected.length; i++) {
        printf("(%d, %d), ", selected.path[i].x, selected.path[i].y);
    }
    printf("\n\n");
    print_path(&selected);
    return 0;
}