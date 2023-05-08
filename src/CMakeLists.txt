cmake_minimum_required(VERSION 3.25)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(src main.c backend/lee.c backend/lee.h backend/common/point.h backend/common/point.c backend/common/path.h backend/common/path.c backend/astar.c backend/astar.h)
