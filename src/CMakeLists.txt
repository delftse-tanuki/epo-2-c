cmake_minimum_required(VERSION 3.25)
project(epo2 C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(thirdparty)
add_subdirectory(backend)
add_subdirectory(cli)
add_subdirectory(gui)
