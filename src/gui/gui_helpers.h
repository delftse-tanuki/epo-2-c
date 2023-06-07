//
// Created by amara on 5/11/23.
//

#ifndef EPO2_GUI_HELPERS_H
#define EPO2_GUI_HELPERS_H

#define NK_INCLUDE_FIXED_TYPES
#include "../thirdparty/nuklear.h"

struct nk_context;

static const struct nk_color nk_red = {255,0,0,255};
static const struct nk_color nk_green = {0,255,0,255};
static const struct nk_color nk_blue = {0,0,255,255};
static const struct nk_color nk_white = {255,255,255,255};
static const struct nk_color nk_black = {0,0,0,255};
static const struct nk_color nk_yellow = {255,255,0,255};

void vertical_spacer(struct nk_context* context, float spacing);

void draw_empty_space(struct nk_context* ctx);

#endif //EPO2_GUI_HELPERS_H
