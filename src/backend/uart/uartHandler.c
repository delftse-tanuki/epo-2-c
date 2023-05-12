//
// Created by Bart Jansen on 08/05/2023.
//
#include <stdint.h>
#include "uart.h"
#include "../common/path.h"
#include "../common/point.h"

HANDLE hSerial;

int facing;
int nextFacing;
char nextInstruction;

enum Instruction {
    FORWARD = 0,
    LEFT = 1,
    RIGHT = 2,
    STOP = 4
};

enum Facing {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

char instructionSet[100];

/* Determine the orientation of the robot based on the last two crossings. */
void determineFacing(struct Point currentPoint, struct Point nextPoint, int *facing) {
    if(currentPoint.y == nextPoint.y) {
        /* The robot is driving vertical */
        if(currentPoint.x < nextPoint.x) {
            /* The robot is driving SOUTH */
            *facing = SOUTH;
        } else {
            /* The robot is driving NORTH */
            *facing = NORTH;
        }

    } else {
        /* The car is driving horizontal */
        if(currentPoint.y < nextPoint.y) {
            /* The robot is driving EAST */
            *facing = EAST;
        } else {
            /* The robot is driving WEST */
            *facing = WEST;
        }
    }
}

void determineNextInstruction() {
    if(facing == nextFacing) {
        nextInstruction = FORWARD;
    } else if(facing == NORTH) {
        switch(nextFacing) {
            case EAST:
                nextInstruction = RIGHT;
                break;
            case WEST:
                nextInstruction = LEFT;
                break;
        }
    } else if (facing == EAST) {
        switch(nextFacing) {
            case NORTH:
                nextInstruction = LEFT;
                break;
            case SOUTH:
                nextInstruction = RIGHT;
                break;
        }
    } else if (facing == SOUTH) {
        switch(nextFacing) {
            case WEST:
                nextInstruction = RIGHT;
                break;
            case EAST:
                nextInstruction = LEFT;
                break;
        }
    } else if(facing == WEST) {
        switch(nextFacing) {
            case NORTH:
                nextInstruction = RIGHT;
                break;
            case SOUTH:
                nextInstruction = LEFT;
                break;
        }
    }
}

void executePath(struct Path path) {
    char byteBuffer[BUFSIZ+1];
    int i = 0;
    int instructionSetIndex = 0;
    determineFacing(path.points[i], path.points[i + 1], &facing);

    for(i; i < path.length; i++) {

        if(path.length - i <= 2) {
            nextInstruction = 4; // Stop instruction
            instructionSet[instructionSetIndex++] = nextInstruction;
            printf("%d, ", nextInstruction);
            break;
        }

        determineFacing(path.points[i + 1], path.points[i + 2], &nextFacing); // Will cause error because i+2 might not exist
        determineNextInstruction();
        instructionSet[instructionSetIndex++] = nextInstruction;

        /* Here we should wait for either confirmation or a mine */

        /* If confirmation, then do the following: */
        facing = nextFacing;
        printf("%d, ", nextInstruction);

        /* If mine, do the following: */
        /* - Update the map with the location of the mine */
        /* - Request a new route from the last crossing, which is still stored in "nextInstruction" */
        /* - Stop this instance of executePath and start a new one, however make sure that the facing is remembered and turned 180 degrees! */
    }

    char buffRead[BUFSIZ+1];
    int lastInstruction = instructionSetIndex;
    instructionSetIndex = 0;

    while (1) {
        readByte(hSerial, buffRead);
        if(buffRead[0] == 32) {
            printf("Received confirmation\n");
            byteBuffer[0] = instructionSet[instructionSetIndex++];
            writeByte(hSerial, byteBuffer);
        }
        if(instructionSetIndex == lastInstruction) {
            break;
        }
        buffRead[0] = 0;
    }

}

void uartHandler() {
    //----------------------------------------------------------
    // Open COMPORT for reading and writing
    //----------------------------------------------------------
    hSerial = CreateFile(COMPORT,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0
    );

    if(hSerial == INVALID_HANDLE_VALUE){
        if(GetLastError()== ERROR_FILE_NOT_FOUND){
            //serial port does not exist. Inform user.
            printf(" serial port does not exist \n");
        }
        //some other error occurred. Inform user.
        printf(" some other error occured. Inform user.\n");
    }

    //----------------------------------------------------------
    // Initialize the parameters of the COM port
    //----------------------------------------------------------

    initSio(hSerial);
}

void closeConnection() {
    CloseHandle(hSerial);
}



