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
    BLOCKED,
    TREASURE
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
        case TREASURE:
            color = nk_rgb(183, 206, 55);
            break;
    }

    nk_stroke_line(canvas, x0, y0, x1, y1, 5.0f, color);
}

bool should_be_highlighted(int x, int y, struct Point point) {
    return (x == point.x && y == point.y);
}

struct Point draw_grid(struct nk_context *ctx, struct Path path, struct PointConnection *mines, int mine_count,
                       bool *selected, struct Point robot_position, struct PointConnection* treasures, int treasure_count) {
    struct Point selection;
    *selected = false;

    struct nk_style_button normal_style = ctx->style.button;
    struct nk_style_button highlighted_style = ctx->style.button;
    highlighted_style.text_normal = nk_rgb(255, 60, 255);
    highlighted_style.border_color = nk_rgb(255, 60, 255);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Live Grid View", NK_TEXT_ALIGN_CENTERED);
    nk_layout_row_static(ctx, 40, 40, 13);

    struct PointConnection connections[MAX_PATH_AMOUNT];
    int connection_count = get_point_connections_from_path(path, connections);

    // Top Station Buttons
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            char label[3];
            int station_index = -1;
            switch (i) {
                case 4:
                    strcpy(label, "S9");
                    station_index = 9;
                    break;
                case 6:
                    strcpy(label, "S8");
                    station_index = 8;
                    break;
                case 8:
                    strcpy(label, "S7");
                    station_index = 7;
                    break;
                default:
                    break;
            }

            if (nk_button_label_styled(ctx, should_be_highlighted(-1, station_index, robot_position) ? &highlighted_style : &normal_style, label)) {
                selection.x = -1;
                selection.y = station_index;
                *selected = true;
            }
        } else {
            draw_empty_space(ctx);
        }
    }

    // Top Station Lines
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            int station_index = -1;
            switch (i) {
                case 4:
                    station_index = 9;
                    break;
                case 6:
                    station_index = 8;
                    break;
                case 8:
                    station_index = 7;
                    break;
                default:
                    break;
            }

            struct Point point;
            point.x = -1;
            point.y = station_index;

            enum LineState line_state = NORMAL;
            if (is_point_in_connection_array(point, mines, mine_count)) {
                line_state = BLOCKED;
            }
            else if (is_point_in_connection_array(point, connections, connection_count)) {
                line_state = HIGHLIGHTED;
            }

            draw_grid_edge(ctx, VERTICAL, line_state);
        } else {
            draw_empty_space(ctx);
        }
    }

    for (int i = 0; i < 9; i++) {
        if (i == 2 || i == 4 || i == 6) {
            char label[4];
            int station_index = -1;
            switch (i) {
                case 2:
                    strcpy(label, "S10");
                    station_index = 10;
                    break;
                case 4:
                    strcpy(label, "S11");
                    station_index = 11;
                    break;
                case 6:
                    strcpy(label, "S12");
                    station_index = 12;
                    break;
                default:
                    break;
            }

            if (nk_button_label_styled(ctx, should_be_highlighted(-1, station_index, robot_position) ? &highlighted_style : &normal_style, label)) {
                selection.x = -1;
                selection.y = station_index;
                *selected = true;
            }
        } else {
            draw_empty_space(ctx);
        }

        if (i == 2 || i == 4 || i == 6) {
            int station_index = -1;
            switch (i) {
                case 2:
                    station_index = 10;
                    break;
                case 4:
                    station_index = 11;
                    break;
                case 6:
                    station_index = 12;
                    break;
                default:
                    break;
            }

            struct Point point;
            point.x = -1;
            point.y = station_index;

            enum LineState line_state = NORMAL;
            if (is_point_in_connection_array(point, mines, mine_count)) {
                line_state = BLOCKED;
            }
            else if (is_point_in_connection_array(point, connections, connection_count)) {
                line_state = HIGHLIGHTED;
            }


            draw_grid_edge(ctx, HORIZONTAL, line_state);
        } else {
            draw_empty_space(ctx);
        }

        for (int j = 0; j < 9; j++) {
            if (i % 2 && j % 2) {
                draw_empty_space(ctx);
            } else if (i % 2) {
                enum LineState line_state = NORMAL;
                struct PointConnection line_connection = get_line_path(i, j);
                if (is_connection_in_array(line_connection, treasures, treasure_count)) {
                    line_state = TREASURE;
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
                if (is_connection_in_array(line_connection, treasures, treasure_count)) {
                    line_state = TREASURE;
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

                if (nk_button_label_styled(ctx, should_be_highlighted(x, y, robot_position) ? &highlighted_style : &normal_style, label)) {
                    selection.x = x;
                    selection.y = y;
                    *selected = true;
                }
            }
        }

        if (i == 2 || i == 4 || i == 6) {
            int station_index = -1;
            switch (i) {
                case 2:
                    station_index = 6;
                    break;
                case 4:
                    station_index = 5;
                    break;
                case 6:
                    station_index = 4;
                    break;
                default:
                    break;
            }

            struct Point point;
            point.x = -1;
            point.y = station_index;

            enum LineState line_state = NORMAL;
            if (is_point_in_connection_array(point, mines, mine_count)) {
                line_state = BLOCKED;
            }
            else if (is_point_in_connection_array(point, connections, connection_count)) {
                line_state = HIGHLIGHTED;
            }

            draw_grid_edge(ctx, HORIZONTAL, line_state);
        } else {
            draw_empty_space(ctx);
        }

        if (i == 2 || i == 4 || i == 6) {
            char label[3];
            int station_index = -1;
            switch (i) {
                case 2:
                    strcpy(label, "S6");
                    station_index = 6;
                    break;
                case 4:
                    strcpy(label, "S5");
                    station_index = 5;
                    break;
                case 6:
                    strcpy(label, "S4");
                    station_index = 4;
                    break;
                default:
                    break;
            }

            if (nk_button_label_styled(ctx, should_be_highlighted(-1, station_index, robot_position) ? &highlighted_style : &normal_style, label)) {
                selection.x = -1;
                selection.y = station_index;
                *selected = true;
            }
        } else {
            draw_empty_space(ctx);
        }
    }

    // Bottom Station Lines
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            int station_index = -1;
            switch (i) {
                case 4:
                    station_index = 1;
                    break;
                case 6:
                    station_index = 2;
                    break;
                case 8:
                    station_index = 3;
                    break;
                default:
                    break;
            }

            struct Point point;
            point.x = -1;
            point.y = station_index;

            enum LineState line_state = NORMAL;
            if (is_point_in_connection_array(point, mines, mine_count)) {
                line_state = BLOCKED;
            }
            else if (is_point_in_connection_array(point, connections, connection_count)) {
                line_state = HIGHLIGHTED;
            }

            draw_grid_edge(ctx, VERTICAL, line_state);
        } else {
            draw_empty_space(ctx);
        }
    }

    // Bottom Station Buttons
    for (int i = 0; i < 13; i++) {
        if (i == 4 || i == 6 || i == 8) {
            char label[3];

            int station_index = -1;
            switch (i) {
                case 4:
                    strcpy(label, "S1");
                    station_index = 1;
                    break;
                case 6:
                    strcpy(label, "S2");
                    station_index = 2;
                    break;
                case 8:
                    strcpy(label, "S3");
                    station_index = 3;
                    break;
                default:
                    break;
            }

            if (nk_button_label_styled(ctx, should_be_highlighted(-1, station_index, robot_position) ? &highlighted_style : &normal_style, label)) {
                selection.x = -1;
                selection.y = station_index;
                *selected = true;
            }
        } else {
            draw_empty_space(ctx);
        }
    }

    return selection;
}