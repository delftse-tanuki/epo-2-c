//
// Created by Jens on 5/3/2023.
//
#include "point.h"

bool point_is_valid(struct Point *point) {
    return (point->x >= 0 && point->x < 13 && point->y >= 0 && point->y < 13);
}