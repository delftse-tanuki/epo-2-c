//
// Created by Bart Jansen on 08/05/2023.
//
#include "uart.h"
#include "../common/path.h"
#include "../common/point.h"


int facing;
int nextFacing;
int nextInstruction;


/* Determine the orientation of the robot based on the last two crossings. */
void determineFacing(struct Point currentPoint, struct Point nextPoint, int *facing) {
    if(currentPoint.x == nextPoint.x) {
        /* The robot is driving vertical */

        if(currentPoint.y < nextPoint.y) {
            /* The robot is driving SOUTH */
            *facing = 2;
        } else {
            /* The robot is driving NORTH */
            *facing = 0;
        }

    } else {
        /* The car is driving horizontal */

        if(currentPoint.x < nextPoint.x) {
            /* The robot is driving EAST */
            *facing = 1;
        } else {
            /* The robot is driving WEST */
            *facing = 3;
        }
    }
}

void determineNextInstruction() {
    if(facing == 3) {
        switch(nextFacing) {
            case 0:
                nextInstruction = 2;
            case 2:
                nextInstruction = 1;
            case 3:
                nextInstruction = 0;
        }
    } else if (nextFacing == facing) {
        nextInstruction = 0;
    } else if (nextFacing > facing) {
        nextInstruction = 2;
    } else {
        nextInstruction = 1;
    }
}

void executePath(/*HANDLE hSerial, */struct Path path) {
    int i = 0;
    determineFacing(path.points[i], path.points[i + 1], &facing);

    for(i; i < path.length; i++) {

        if(path.length <= 2) {
            //writeByte(hSerial, 00) /* If there are two points left, the instruction is always go forward */
            break;
        }

        determineFacing(path.points[i + 1], path.points[i + 2], &nextFacing); // Will cause error because i+2 might not exist
        determineNextInstruction();
        //writeByte(hSerial, nextInstruction);

        /* Here we should wait for either confirmation or a mine */

        /* If confirmation, then do the following: */
        facing = nextFacing;
        printf("%d, ", nextInstruction);

        /* If mine, do the following: */
        /* - Update the map with the location of the mine */
        /* - Request a new route from the last crossing, which is still stored in "nextInstruction" */
        /* - Stop this instance of executePath and start a new one, however make sure that the facing is remembered and turned 180 degrees! */
    }

}

void uartHandler() {
    struct Path path;
    path.points[0] = (struct Point){6,0};
    path.points[1] = (struct Point){6,2};
    path.points[2] = (struct Point){4,2};
    path.points[3] = (struct Point){4,4};
    path.length = 4;
    path.turns = 2;

    executePath(path);
}



