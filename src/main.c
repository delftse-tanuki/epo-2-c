#include <stdio.h>
#include <sysinfoapi.h>

#include "backend/lee.h"

int main() {
    for(int i = 0; i < 169; i++) {
        if(i % 13 == 0) {
            printf("\n");
        }
        printf("%d,\t", lee_map[i]);
    }
    long long start = GetTickCount();
    lee(0, 0, 1, 3);
    long long end = GetTickCount();
    printf("\n");
    for(int i = 0; i < 169; i++) {
        if(i % 13 == 0) {
            printf("\n");
        }
        printf("%d,\t", lee_map[i]);
    }

    printf("\n\nTime: %lld", end - start);
    return 0;
}
