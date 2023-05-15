add_executable(epo2cli main.c cli.c cli.h)

target_link_libraries(epo2cli backend)