#include <stdio.h>

#include "../backend/algorithms/lee.h"
#include "cli.h"
#include "../backend/mazeRouter.h"
#include "../backend/uart/uartHandler.h"
#include "../backend/uart/uart.h"
/*
int main()
{
    HANDLE hSerial;


    char byteBuffer[BUFSIZ+1];
    char readBuffer[BUFSIZ+1];

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

    while ( 1 ) {
        gets(byteBuffer);

        if (byteBuffer[0] == 'q') // end the loop by typing 'q'
            break;

        writeByte(hSerial, byteBuffer);
        readByte(hSerial, readBuffer);
    }

    printf("ZIGBEE IO DONE!\n");
    return 0;

    CloseHandle(hSerial);
}*/


int main() {
    reset_lee_maze();
    initConnection();

    struct Point sourceStation = index_to_station(6);
    struct Point destinationStation = index_to_station(8);
    struct Path path = calculate_route(sourceStation, destinationStation);
    executePath(path, 0, 1);

    /*
    for (int i = 1; i <= 12; i++) {
        for(int j = 12; j >= 1; j--) {
            struct Path path = calculate_route(index_to_station(i), index_to_station(j));
            printf("Path from (%d, %d) to (%d, %d) (%d Turns): ", index_to_station(i).x, index_to_station(i).y, index_to_station(j).x, index_to_station(j).y, path.turns);
            for(int i = 0; i < path.length; i++) {
                printf("(%d, %d), ", path.points[i].x, path.points[i].y);
            }
            printf("\n");
            reset_lee_maze();
        }
    }*/

    init_CLI();
    closeConnection();
    return 0;
}