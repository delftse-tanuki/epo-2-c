//
// Created by Jens on 5/8/2023.
//

#ifndef SRC_UART_H
#define SRC_UART_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define COMPORT "COM4"
#define BAUDRATE CBR_9600

void initSio(HANDLE hSerial);
int readByte(HANDLE hSerial, char *buffRead);
int writeByte(HANDLE hSerial, char *buffWrite);

#endif //SRC_UART_H
