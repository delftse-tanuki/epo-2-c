#ifndef EPO2_FAKE_UART_HANDLER_H
#define EPO2_FAKE_UART_HANDLER_H

#ifdef FAKE_UART

#include "../common/path.h"
#include "UARTInstructions.h"

static bool simulate_uart_failure = false;
extern bool add_treasures;

void initUART();
void executePath(struct Path path, __attribute__((unused)) int isLast, void (*path_ended)(enum PathExecutionResult), void (*robot_moved)(struct PointConnection movement));
void closeConnection();

#endif
#endif //EPO2_FAKE_UART_HANDLER_H
