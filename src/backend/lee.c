//
// Created by Jens on 4/24/2023.
//

#include "lee.h"

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

void lee_path(int sourceIndex, int destinationIndex) {
    int current_index = sourceIndex;
    int target_index = destinationIndex;
    while(current_index != target_index) {
        for(int direction = NORTH; direction <= WEST; direction++) {
            int neighbour_index = direction_index(sourceIndex, direction);
            if(lee_map[neighbour_index] == lee_map[current_index] - 1) {
                current_index = neighbour_index;
                lee_path(current_index, target_index);
            }
        }
    }
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
            if(lee_map[neighbour_index] == 0) {
                lee_map[neighbour_index] = lee_map[current_index] + 1;
                queue[queue_end++] = neighbour_index;
            }
        }
    }
    //lee_path(sourceIndex, destinationIndex);
}

void lee(int sourceX, int sourceY, int destinationX, int destinationY) {
    int sourceIndex = crossing_mask[sourceX][sourceY];
    int destinationIndex = crossing_mask[destinationX][destinationY];
    lee_algorithm(sourceIndex, destinationIndex);
}