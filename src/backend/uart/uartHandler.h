//
// Created by Jens on 5/11/2023.
//

#ifndef SRC_UARTHANDLER_H
#define SRC_UARTHANDLER_H

#include "UARTInstructions.h"

void initUART();
void executePath(struct Path path, void (*path_ended)(enum PathExecutionResult));
void closeConnection();

#endif //SRC_UARTHANDLER_H
