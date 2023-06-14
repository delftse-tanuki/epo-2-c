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
        challenges/challenge_signals.h
        challenges/challenge_signals.c
        challenges/challenge_a.h
        challenges/challenge_a.c
        challenges/challenge_b.h
        challenges/challenge_b.c
        challenges/challenge_c.c
        challenges/challenge_c.h)

if (UNIX)
    target_compile_definitions(backend PUBLIC FAKE_UART UNIX)
    # Move this statement below OUTSIDE of this if() block to test fake uart on Windows.
    target_sources(backend PRIVATE
            uart/fake_uart_handler.c
            uart/fake_uart_handler.h)
endif()

if (WIN32)
    # Comment out the statement below to test fake uart on Windows.
    target_sources(backend PRIVATE
            uart/uart.c
            uart/uart.h
            uart/uartHandler.c
            uart/uartHandler.h
            uart/UARTInstructions.h)
endif ()