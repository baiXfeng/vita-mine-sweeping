//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_CONTEXT_H
#define SDL2_UI_CONTEXT_H

#include "common/grid.h"
#include "common/observer.h"
#include "tile.h"

class ContextObserver {
public:
    virtual ~ContextObserver() {}
    virtual void onTimeModify(float seconds) {}
    virtual void onMineNumberModify(uint32_t number) {}
};

class Context : public mge::Observer<ContextObserver> {
public:
    Context() {
        flag = false;
        finished = false;
        mine_number = 0;
        max_mine_number = 0;
        seconds = 0.0f;
    }
    bool flag;      // 标旗模式
    bool finished;  // 游戏结束
    uint32_t mine_number;   // 剩余地雷数量
    uint32_t max_mine_number;   // 地雷总数
    float seconds;  // 游戏时间
    mge::Grid<Tile> grid;
};

#define ENUM(NAME, VALUE) static const int NAME = VALUE

namespace EVENT_ID {
    ENUM(GAME_START, 1000);
    ENUM(GAME_OVER, 1001);
    ENUM(GAME_WIN, 1002);
};

#endif //SDL2_UI_CONTEXT_H
