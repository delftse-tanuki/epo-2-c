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
        )

if (WIN32)
target_sources(backend PRIVATE
        uart/uart.c
        uart/uart.h
        uart/uartHandler.c
        uart/uartHandler.h
        uart/UARTInstructions.h)
endif()