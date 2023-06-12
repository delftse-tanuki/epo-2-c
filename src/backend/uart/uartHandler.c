//
// Created by Bart Jansen on 08/05/2023.
//
#include <stdint.h>
#include "uart.h"
#include "../common/path.h"
#include "UARTInstructions.h"
#include "../challenges/challenge_signals.h"
#include "../../gui/gui.h"

HANDLE hSerial;

int facing;
int nextFacing;
char nextInstruction;

struct UARTInstruction instructionSet[100];

/**
 * Determine the direction of the path between 2 neighbouring points.
 *
 * @param startPoint The start point.
 * @param endPoint The end point.
 */
void determineFacing(struct Point startPoint, struct Point endPoint, int *facing) {
    if(startPoint.y == endPoint.y) {
        /* The robot is driving vertical */
        if(startPoint.x < endPoint.x) {
            /* The robot is driving SOUTH */
            *facing = SOUTH;
        } else {
            /* The robot is driving NORTH */
            *facing = NORTH;
        }
        /* The car is driving horizontal */
    } else if(startPoint.y < endPoint.y) {
        /* The robot is driving EAST */
        *facing = EAST;
    } else {
        /* The robot is driving WEST */
        *facing = WEST;
    }
}

/**
 * Determine the next instruction for the based on the current orientation.
 */
void determineNextInstruction() {
    if(facing == nextFacing) {
        nextInstruction = FORWARD;
    } else if(
        (facing == NORTH && nextFacing == EAST) || 
        (facing == EAST && nextFacing == SOUTH) || 
        (facing == SOUTH && nextFacing == WEST) || 
        (facing == WEST && nextFacing == NORTH)
    ) {
        nextInstruction = RIGHT;
    } else if (
        (facing == NORTH && nextFacing == EAST) || 
        (facing == EAST && nextFacing == SOUTH) || 
        (facing == SOUTH && nextFacing == WEST) || 
        (facing == WEST && nextFacing == NORTH)
    ) {
        nextInstruction = LEFT;
    }
}

void executeInstructions(struct UARTInstruction instructions[], int length, int isLast, void (*path_ended)(enum PathExecutionResult)) {
    char byteBuffer[BUFSIZ+1];
    char buffRead[BUFSIZ+1];
    int instructionSetIndex = 0;
    int lastInstruction = length;
    if(isLast == 2) {
        byteBuffer[0] = STOP;
        writeByte(hSerial, byteBuffer);
    }
    get_robot_state()->next_instruction = instructions[instructionSetIndex].instruction;
    get_robot_state()->facing = instructions[instructionSetIndex].facing;
    while (1) {
        get_robot_state()->data_received = false;
        readByte(hSerial, buffRead);
        if(buffRead[0] == 32) {
            get_robot_state()->data_received = true;
            get_robot_state()->last_reported_position = lee_to_index(instructions[instructionSetIndex + 1].point);
            get_robot_state()->next_instruction = instructions[instructionSetIndex + 1].instruction;
            get_robot_state()->facing = instructions[instructionSetIndex + 1].facing;
            printf("Received confirmation\n");
            byteBuffer[0] = instructions[instructionSetIndex].instruction;
            writeByte(hSerial, byteBuffer);
            instructionSetIndex++;
        }
        if(instructionSetIndex == lastInstruction - 1) {
            if(isLast == 1) {
                byteBuffer[0] = STOP;
                Sleep(500);
                writeByte(hSerial, byteBuffer);
            }
            path_ended(SUCCESS);
            break;
        }
        if(buffRead[0] == 33) {
            printf("Received mine\n");
            path_ended(MINE);
            break;
        }
        buffRead[0] = 0;
#ifdef GUI
        gui_update();
#endif
    }
}

/**
 * Generate and execute the instructions for the robot.
 * @param path The path to follow.
 */
void executePath(struct Path path, int isLast, void (*path_ended)(enum PathExecutionResult)) {
    get_robot_state()->current_path = path;
    get_robot_state()->last_reported_position = lee_to_index(path.points[0]);
    char byteBuffer[BUFSIZ+1];
    int i = 0;
    int instructionSetIndex = 0;
    determineFacing(path.points[i], path.points[i + 1], &facing);

    for(i; i < path.length; i++) {

        if(path.length - i <= 2) {
            nextInstruction = STOP;
            instructionSet[instructionSetIndex++].instruction = nextInstruction;
            instructionSet[instructionSetIndex].point = path.points[i + 1];
            instructionSet[instructionSetIndex].facing = facing;
            printf("%d, ", nextInstruction);
            break;
        }

        determineFacing(path.points[i + 1], path.points[i + 2], &nextFacing); // Will cause error because i+2 might not exist
        determineNextInstruction();
        instructionSet[instructionSetIndex++].instruction = nextInstruction;
        instructionSet[instructionSetIndex].point = path.points[i + 1];
        instructionSet[instructionSetIndex].facing = facing;

        /* Here we should wait for either confirmation or a mine */

        /* If confirmation, then do the following: */
        facing = nextFacing;
        printf("%d, ", nextInstruction);

        /* If mine, do the following: */
        /* - Update the map with the location of the mine */
        /* - Request a new route from the last crossing, which is still stored in "nextInstruction" */
        /* - Stop this instance of executePath and start a new one, however make sure that the facing is remembered and turned 180 degrees! */
    }
    executeInstructions(&instructionSet, instructionSetIndex, isLast, path_ended);
}

/**
 * Initialize the UART connection
 * @param hSerial
 */
void initUART() {
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



