//
// Created by Jens on 5/8/2023.
//
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../backend/mazeRouter.h"
#include "../backend/uart/uartHandler.h"

int running = 1;

/**
 * Starts challenge A or B
 */
void challengeAB() {
    printf("Starting challenge A/B...\n");
    printf("What is the source station (x): ");
    int source;
    scanf("%d", &source);
    if (source < 1 || source > 12) {
        printf("Invalid source station: %d\n", source);
        return;
    }
    printf("What is the destination station (x): ");
    int destination;
    scanf("%d", &destination);
    if (destination < 1 || destination > 12) {
        printf("Invalid destination station: %d\n", destination);
        return;
    }

    struct Path path = calculate_route(index_to_station(source), index_to_station(destination));

    printf("Selected path (%d Turns): ", path.turns);
    for(int i = 0; i < path.length; i++) {
        printf("(%d, %d), ", path.points[i].x, path.points[i].y);
    }

    //executePath(path);
    printf("\n\n");
}

/**
 * Starts challenge C
 */
void challengeC() {

}

/**
 * Handles the start command and prompts the user for the challenge
 */
void handle_start() {
    printf("Which challenge are you starting (A/B/C): ");
    char challenge;
    scanf(" %c", &challenge);
    if (toupper(challenge) == 'A' || toupper(challenge) == 'B') {
        challengeAB();
    } else if (toupper(challenge) == 'C') {
        printf("Starting challenge C...\n");
    } else {
        printf("Unknown challenge: %c\n", challenge);
    }
}

/**
 * Starts the CLI
 */
void init_CLI() {
    printf("Welcome to the MoRoBoBo CLI!\n");
    while (running) {
        printf(">> ");
        char input[100];
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) {
            printf("Exiting CLI...\n");
            running = 0;
        } else if (strcmp(input, "start") == 0) {
            handle_start();
        } else {
            printf("Unknown command: %s\n", input);
        }
    }
}