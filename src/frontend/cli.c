//
// Created by Jens on 5/8/2023.
//
#include <stdio.h>
#include <string.h>

int running = 0;

void init_CLI() {
    printf("Welcome to the MoRoBoBo CLI!\n");
    while (running) {
        printf(">> ");
        char input[100];
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) {
            printf("Exiting CLI...\n");
            running = 0;
        } else {
            printf("Unknown command: %s\n", input);
        }
    }
}