//
// Created by amara on 5/11/23.
//

#include "gui_helpers.h"

#define NK_INCLUDE_FIXED_TYPES
#include "../thirdparty/nuklear.h"

void vertical_spacer(struct nk_context* context, float spacing) {
    nk_layout_row_dynamic(context, spacing, 1);
    nk_spacing(context, 1);
}

void draw_empty_space(struct nk_context* ctx) {
    struct nk_rect space;
    nk_widget(&space, ctx);
}