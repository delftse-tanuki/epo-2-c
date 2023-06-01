//
// Created by amara on 5/8/23.
//

#include "grid.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define NK_INCLUDE_FIXED_TYPES

#include "../thirdparty/nuklear.h"
#include "gui_helpers.h"

enum LineKind {
    VERTICAL,
    HORIZONTAL
};

enum LineState {
    NORMAL,
    HIGHLIGHTED,
    CURRENT,
    BLOCKED
};

struct PointConnection get_line_path(int i, int j) {
    struct PointConnection connection;

    if (i % 2) {
        // Vertical Line
        connection.point1.x = (int) floorf((float) j / 2);
        connection.point2.x = (int) floorf((float) j / 2);
        connection.point1.y = (int) floorf((float) (i - 1) / 2);
        connection.point2.y = (int) floorf((float) (i + 1) / 2);
    } else {
        // Horizontal Line
        connection.point1.y = (int) floorf((float) i / 2);
        connection.point2.y = (int) floorf((float) i / 2);
        connection.point1.x = (int) floorf((float) (j - 1) / 2);
        connection.point2.x = (int) floorf((float) (j + 1) / 2);
    }

    return connection;
}

void draw_grid_edge(struct nk_context *ctx, enum LineKind line_kind, enum LineState line_state) {
    struct nk_command_buffer *canvas;
    canvas = nk_window_get_canvas(ctx);

    struct nk_rect space;
    enum nk_widget_layout_states state;
    state = nk_widget(&space, ctx);
    if (!state) {
        return;
    }

    float x0, x1, y0, y1;
    struct nk_color color;

    if (line_kind == HORIZONTAL) {
        x0 = space.x;
        x1 = space.x + space.w;
        y0 = space.y + space.h / 2;
        y1 = space.y + space.h / 2;
    } else {
        x0 = space.x + space.w / 2;
        x1 = space.x + space.w / 2;
        y0 = space.y;
        y1 = space.y + space.h;
    }

    switch (line_state) {
        case NORMAL:
            color = nk_rgb(255, 255, 255);
            break;
        case HIGHLIGHTED:
            color = nk_rgb(60, 180, 180);
            break;
        case BLOCKED:
            color = nk_rgb(255, 60, 60);
            break;
        case CURRENT:
            color = nk_rgb(255, 60, 255);
            break;
    }

    nk_stroke_line(canvas, x0, y0, x1, y1, 5.0f, color);
}

struct Point draw_grid(struct nk_context *ctx, struct Path path, struct PointConnection *mines, int mine_count,
                       bool *selected, struct PointConnection robot_position) {
    struct Point selection;

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Live Grid View", NK_TEXT_ALIGN_CENTERED);
    nk_layout_row_static(ctx, 40, 40, 13);

    struct PointConnection connections[MAX_PATH_AMOUNT];
    int connection_count = get_point_connections_from_path(path, connections);

    // Top Station Buttons
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            char label[3];
            strcpy(label, i == 4 ? "S6" : (i == 6 ? "S8" : "S7"));
            nk_button_label(ctx, label);
        } else {
            draw_empty_space(ctx);
        }
    }

    // Top Station Lines
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            draw_grid_edge(ctx, VERTICAL, NORMAL);
        } else {
            draw_empty_space(ctx);
        }
    }

    for (int i = 0; i < 9; i++) {
        if (i == 2 || i == 4 || i == 6) {
            char label[4];
            strcpy(label, i == 2 ? "S10" : (i == 4 ? "S11" : "S12"));
            nk_button_label(ctx, label);
        } else {
            draw_empty_space(ctx);
        }

        if (i == 2 || i == 4 || i == 6) {
            draw_grid_edge(ctx, HORIZONTAL, NORMAL);
        } else {
            draw_empty_space(ctx);
        }

        for (int j = 0; j < 9; j++) {
            if (i % 2 && j % 2) {
                draw_empty_space(ctx);
            } else if (i % 2) {
                enum LineState line_state = NORMAL;
                struct PointConnection line_connection = get_line_path(i, j);
                if (is_point_connection_equal(line_connection, robot_position)) {
                    line_state = CURRENT;
                }
                else if (is_connection_in_array(line_connection, mines, mine_count)) {
                    line_state = BLOCKED;
                }
                else if (is_connection_in_array(line_connection, connections, connection_count)) {
                    line_state = HIGHLIGHTED;
                }
                draw_grid_edge(ctx, VERTICAL, line_state);
            } else if (j % 2) {
                enum LineState line_state = NORMAL;
                struct PointConnection line_connection = get_line_path(i, j);
                if (is_point_connection_equal(line_connection, robot_position)) {
                    line_state = CURRENT;
                }
                else if (is_connection_in_array(line_connection, mines, mine_count)) {
                    line_state = BLOCKED;
                }
                else if (is_connection_in_array(line_connection, connections, connection_count)) {
                    line_state = HIGHLIGHTED;
                }
                draw_grid_edge(ctx, HORIZONTAL, line_state);
            } else {
                char label[5];
                int x = (int) floorf((float) j / 2);
                int y = (int) floorf((float) i / 2);

                snprintf(label, 5, "%d, %d", x, y);

                if (nk_button_label(ctx, label)) {
                    selection.x = x;
                    selection.y = y;
                    *selected = true;
                }
            }
        }

        if (i == 2 || i == 4 || i == 6) {
            draw_grid_edge(ctx, HORIZONTAL, NORMAL);
        } else {
            draw_empty_space(ctx);
        }

        if (i == 2 || i == 4 || i == 6) {
            char label[3];
            strcpy(label, i == 2 ? "S6" : (i == 4 ? "S5" : "S4"));
            nk_button_label(ctx, label);
        } else {
            draw_empty_space(ctx);
        }
    }

    // Top Station Lines
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            draw_grid_edge(ctx, VERTICAL, NORMAL);
        } else {
            draw_empty_space(ctx);
        }
    }

    // Top Station Buttons
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            char label[3];
            strcpy(label, i == 4 ? "S1" : (i == 6 ? "S2" : "S3"));
            nk_button_label(ctx, label);
        } else {
            draw_empty_space(ctx);
        }
    }

    return selection;
}