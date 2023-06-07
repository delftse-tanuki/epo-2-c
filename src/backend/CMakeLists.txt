add_library(backend OBJECT
        algorithms/astar.c
        algorithms/astar.h
        algorithms/lee.c
        algorithms/lee.h
        common/path.c
        common/path.h
        common/point.c
        common/point.h
        mazeRouter.c
        mazeRouter.h
        robot_state.h
        challenges/challenge_a.h
        challenges/challenge_signals.h
        challenges/challenge_signals.c
        challenges/challenge_a.c
        uart/fake_uart_handler.h
        uart/fake_uart_handler.c
        )

if (UNIX)
    target_compile_definitions(backend PUBLIC FAKE_UART UNIX)
endif()

if (WIN32)
    target_sources(backend PRIVATE
            uart/uart.c
            uart/uart.h
            uart/uartHandler.c
            uart/uartHandler.h
            uart/UARTInstructions.h)
endif ()