#ifndef EPO2_FAKE_UART_HANDLER_H
#define EPO2_FAKE_UART_HANDLER_H

#include "../common/path.h"
#include "UARTInstructions.h"

static bool simulate_uart_failure = false;

void initUART();
void executePath(struct Path path, void (*path_ended)(enum PathExecutionResult));
void closeConnection();

#endif //EPO2_FAKE_UART_HANDLER_H
