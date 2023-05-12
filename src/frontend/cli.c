//
// Created by Jens on 5/8/2023.
//
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../backend/mazeRouter.h"
#include "../backend/uart/uartHandler.h"

int running = 1;

void challenge_a() {
    printf("Starting challenge A/B...\n");
    printf("What is the source station (x): ");
    int source;
    scanf("%d", &source);
    printf("What is the destination station (x): ");
    int destination;
    scanf("%d", &destination);
    struct Point sourceStation = index_to_station(source);
    struct Point destinationStation = index_to_station(destination);
    struct PathList pathList = calculate_route(&sourceStation, &destinationStation);
    struct Path best_path = select_path(&pathList);

    printf("Paths found: %d\n", pathList.length);
    printf("Selected path (%d Turns): ", best_path.turns);
    for(int i = 0; i < best_path.length; i++) {
        printf("(%d, %d), ", best_path.points[i].x, best_path.points[i].y);
    }
    executePath(best_path);
    printf("\n\n");
}

void challenge_b() {
    uartHandler();
}

void challenge_c() {

}

void handle_start() {
    printf("Which challenge are you starting (A/B/C): ");
    char challenge;
    scanf(" %c", &challenge);
    if (toupper(challenge) == 'A' ) {
        challenge_a();
    } else if (toupper(challenge) == 'B') {
        challenge_b();
    } else if (toupper(challenge) == 'C') {
        printf("Starting challenge C...\n");
    } else {
        printf("Unknown challenge: %c\n", challenge);
    }
}

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