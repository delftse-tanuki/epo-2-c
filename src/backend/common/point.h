//
// Created by Jens on 5/3/2023.
//

#ifndef SRC_POINT_H
#define SRC_POINT_H

#include <stdbool.h>

struct Point {
    int x;
    int y;
};

bool point_is_valid(struct Point *point);

#endif //SRC_POINT_H
