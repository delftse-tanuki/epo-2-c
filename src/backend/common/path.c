//
// Created by Jens on 5/3/2023.
//
#include <stdio.h>
#include <string.h>
#include "path.h"

/**
 * Calculates the turns in a path
 * @param path
 * @return the number of turns
 */
int calc_turns(struct Path *path) {
    int turns = 0;
    for(int i = 0; i < path->length - 2; i++) {
        int x1 = path->points[i].x;
        int y1 = path->points[i].y;
        int x2 = path->points[i + 1].x;
        int y2 = path->points[i + 1].y;
        int x3 = path->points[i + 2].x;
        int y3 = path->points[i + 2].y;
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