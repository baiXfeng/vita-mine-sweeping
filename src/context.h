//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_CONTEXT_H
#define SDL2_UI_CONTEXT_H

#include "common/grid.h"
#include "tile.h"

class Context {
public:
    mge::Grid<Tile> map;
};

#endif //SDL2_UI_CONTEXT_H
