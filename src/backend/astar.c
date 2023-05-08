#include <limits.h>
#include <stdbool.h>
#include "astar.h"
#include <stdio.h>

#include "stdlib.h"

const int ASTAR_GRID_SIZE = 5;

int h(struct Point start, struct Point goal) {
    int dx = abs(goal.x - start.x);
    int dy = abs(goal.y - start.y);
    return dx + dy;
}

bool is_neighbor_accessible(struct Point point1, struct Point point2, struct PointConnection mines[ASTAR_GRID_SIZE * ASTAR_GRID_SIZE], int mine_count) {
    for (int i = 0; i < mine_count; i++) {
        if (   (point1.x == mines[i].point1.x && point1.y == mines[i].point1.y
            &&  point2.x == mines[i].point2.x && point2.y == mines[i].point2.y)
            || (point1.x == mines[i].point2.x && point1.y == mines[i].point2.y
            &&  point2.x == mines[i].point1.x && point2.y == mines[i].point1.y)) {
            return false;
        }
    }
    return true;
}

int get_point_neighbors(struct Point point, struct Point *result, struct PointConnection mines[ASTAR_GRID_SIZE * ASTAR_GRID_SIZE], int mine_count) {
    int result_count = 0;
    if (point.x > 0) {
        result[result_count].x = point.x - 1;
        result[result_count].y = point.y;
        if (is_neighbor_accessible(point, result[result_count], mines, mine_count)) {
            result_count++;
        }
    }

    if (point.x < ASTAR_GRID_SIZE - 1) {
        result[result_count].x = point.x + 1;
        result[result_count].y = point.y;
        if (is_neighbor_accessible(point, result[result_count], mines, mine_count)) {
            result_count++;
        }
    }

    if (point.y > 0) {
        result[result_count].x = point.x;
        result[result_count].y = point.y - 1;
        if (is_neighbor_accessible(point, result[result_count], mines, mine_count)) {
            result_count++;
        }
    }

    if (point.y < ASTAR_GRID_SIZE - 1) {
        result[result_count].x = point.x;
        result[result_count].y = point.y + 1;
        if (is_neighbor_accessible(point, result[result_count], mines, mine_count)) {
            result_count++;
        }
    }

    return result_count;
}

void free_memory(struct PointListEntry* open_set, struct Point* came_from[ASTAR_GRID_SIZE][ASTAR_GRID_SIZE]) {
    struct PointListEntry* current = open_set;
    struct PointListEntry* last;
    while (current != NULL) {
        last = current;
        current = current->next;
        free(last);
    }

    int i, j;
    for (i = 0; i < ASTAR_GRID_SIZE; i++) {
        for (j = 0; j < ASTAR_GRID_SIZE; j++) {
            if (came_from[i][j] != NULL) {
                free(came_from[i][j]);
            }
        }
    }
}

struct Path reconstruct_path(struct Point current, struct Point* came_from[ASTAR_GRID_SIZE][ASTAR_GRID_SIZE], struct PointListEntry* open_set) {
    struct Path path;

    path.points[0].x = current.x;
    path.points[0].y = current.y;

    int path_length = 1;

    while (came_from[current.x][current.y] != NULL) {
        current = *came_from[current.x][current.y];
        path.points[path_length++] = current;
    }

    path.length = path_length;
    path.turns = calc_turns(&path);
    free_memory(open_set, came_from);
    return path;
}

struct Path get_path(struct Point start, struct Point goal, struct PointConnection mines[ASTAR_GRID_SIZE * ASTAR_GRID_SIZE], int mine_count) {
    struct PointListEntry *open_set = malloc(sizeof(struct PointListEntry));
    open_set->point = start;

    // For use in for loops.
    int i, j;

    struct Point* came_from[ASTAR_GRID_SIZE][ASTAR_GRID_SIZE];
    for (i = 0; i < ASTAR_GRID_SIZE; i++) {
        for (j = 0; j < ASTAR_GRID_SIZE; j++) {
            came_from[i][j] = NULL;
        }
    }

    // Initialize g_score to 'infinity' by default.
    int g_score[ASTAR_GRID_SIZE][ASTAR_GRID_SIZE];
    for (i = 0; i < ASTAR_GRID_SIZE; i++) {
        for (j = 0; j < ASTAR_GRID_SIZE; j++) {
            g_score[i][j] = INT_MAX;
        }
    }

    g_score[start.x][start.y] = 0;

    // Initialize f_score to 'infinity' by default.
    int f_score[ASTAR_GRID_SIZE][ASTAR_GRID_SIZE];
    for (i = 0; i < ASTAR_GRID_SIZE; i++) {
        for (j = 0; j < ASTAR_GRID_SIZE; j++) {
            f_score[i][j] = INT_MAX;
        }
    }

    f_score[start.x][start.y] = h(start, goal);

    while (open_set != NULL) {
        struct Point current;
        current.x = -1;
        current.y = -1;

        struct PointListEntry *entry = open_set;
        while (entry != NULL) {
            if (f_score[entry->point.x][entry->point.y] < f_score[current.x][current.y]
                    || (current.x == -1 && current.y == -1)) {
                current = entry->point;
            }
            entry = entry->next;
        }

        if (current.x == goal.x && current.y == goal.y) {
            return reconstruct_path(current, came_from, open_set);
        }

        // Erase current from open_set.
        struct PointListEntry *entry_to_delete = NULL;
        struct PointListEntry *previous_entry = NULL;
        entry = open_set;
        while (entry != NULL) {
            if (entry->point.x == current.x && entry->point.y == current.y) {
                entry_to_delete = entry;
                if (previous_entry != NULL) {
                    previous_entry->next = entry->next;
                }
                else if (entry->next != NULL) {
                    open_set = entry->next;
                }
            }

            previous_entry = entry;
            entry = entry->next;
        }
        if (entry_to_delete == open_set) {
            open_set = NULL;
        }
        free(entry_to_delete);

        struct Point neighbors[4];
        int neighbour_count = get_point_neighbors(current, neighbors, mines, mine_count);
        for (i = 0; i < neighbour_count; i++) {
            int tentative_g_score = g_score[current.x][current.y] + 1; // Add weight here.
            if (tentative_g_score < g_score[neighbors[i].x][neighbors[i].y]) {
                came_from[neighbors[i].x][neighbors[i].y] = malloc(sizeof(struct PointListEntry));
                *came_from[neighbors[i].x][neighbors[i].y] = current;
                g_score[neighbors[i].x][neighbors[i].y] = tentative_g_score;
                f_score[neighbors[i].x][neighbors[i].y] = tentative_g_score + h(neighbors[i], goal);

                // Add neighbor to open_set if it wasn't already there.
                entry = open_set;
                bool is_in_open_set = false;
                while (entry != NULL) {
                    if (entry->point.x == neighbors[i].x && entry->point.y == neighbors[i].y) {
                        is_in_open_set = true;
                    }
                    entry = entry->next;
                }
                if (!is_in_open_set) {
                    struct PointListEntry* new_entry = malloc(sizeof(struct PointListEntry));
                    new_entry->point = neighbors[i];
                    if (open_set == NULL) {
                        open_set = new_entry;
                    }
                    else {
                        struct PointListEntry* head = open_set;
                        open_set = new_entry;
                        new_entry->next = head;
                    }
                }
            }
        }
    }

    free_memory(open_set, came_from);
    struct Path result;
    result.length = 0;
    return result;
}

