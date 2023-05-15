//
// Created by amara on 5/8/23.
//

#ifndef EPO2_GRID_H
#define EPO2_GRID_H

#include "../backend/common/path.h"

struct nk_context;

struct Point draw_grid(struct nk_context *ctx, struct Path path, struct PointConnection* mines, int mine_count, bool* selected,
                       struct PointConnection robot_position);

#endif //EPO2_GRID_H
