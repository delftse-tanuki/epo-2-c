cmake_minimum_required(VERSION 3.25)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(src main.c backend/algorithms/lee.c backend/algorithms/lee.h backend/common/point.h backend/common/point.c backend/common/path.h backend/common/path.c backend/algorithms/astar.c backend/algorithms/astar.h backend/uart/uart.h frontend/cli.c frontend/cli.h backend/mazeRouter.c backend/mazeRouter.h)
