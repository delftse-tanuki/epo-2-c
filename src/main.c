#include <stdio.h>

#include "backend/algorithms/lee.h"
#include "frontend/cli.h"
#include "backend/mazeRouter.h"
#include "backend/uart/uartHandler.h"
#include "backend/uart/uart.h"

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
}

/*
int main() {
    reset_lee_maze();
    uartHandler();

    add_mine(&(struct Point){6,7});
    add_mine(&(struct Point){7,6});
    add_mine(&(struct Point){8,5});
    add_mine(&(struct Point){9,4});
    add_mine(&(struct Point){10,3});

    struct Point sourceStation = {8, 6};
    struct Point destinationStation = {8, 4};
    struct PathList pathList = calculate_route(&sourceStation, &destinationStation);
    struct Path best_path = select_path(&pathList);
    executePath(best_path);

    init_CLI();
    closeConnection();
    return 0;
}*/