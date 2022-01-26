//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_CONTEXT_H
#define SDL2_UI_CONTEXT_H

#include "common/grid.h"
#include "common/observer.h"
#include "tile.h"

class GameStateObserver {
public:
    virtual ~GameStateObserver() {}
    virtual void onTimeModify(float seconds) {}
    virtual void onMineNumberModify(uint32_t number) {}
};

class GameState : public mge::Observer<GameStateObserver> {
public:
    GameState() {
        flag = false;
        finished = false;
        paused = false;
        first_click = true;
        mine_number = 0;
        max_mine_number = 0;
        seconds = 0.0f;
    }
    bool flag;      // 标旗模式
    bool finished;  // 游戏结束
    bool paused;    // 游戏暂停
    bool first_click;   // 首次点击
    uint32_t mine_number;   // 剩余地雷数量
    uint32_t max_mine_number;   // 地雷总数
    float seconds;  // 游戏时间

    void notify_time() {
        notify(&GameStateObserver::onTimeModify, seconds);
    }
    void notify_mine_number() {
        notify(&GameStateObserver::onMineNumberModify, mine_number);
    }
};

class GameConfig {
public:
    GameConfig() {
        max_map_width = 64;
        max_map_height = 64;
        min_map_width = 2;
        min_map_height = 2;
    }
    uint32_t max_map_width;    // 最大地图宽度
    uint32_t max_map_height;   // 最大地图高度
    uint32_t min_map_width;    // 最小地图宽度
    uint32_t min_map_height;   // 最小地图高度
};

class GameSetting {
public:
    GameSetting() {
        map_width = 10;
        map_height = 10;
        mine_number = 10 * 10 * 0.12f;
    }
    uint32_t map_width;     // 地图宽度
    uint32_t map_height;    // 地图高度
    uint32_t mine_number;   // 地雷数量
};

class Context {
public:
    GameConfig const config;    // 游戏静态配置
    GameSetting setting;        // 游戏设置
    GameState state;            // 游戏状态
    mge::Grid<Tile> grid;       // 地图数据
};

#define ENUM(NAME, VALUE) static const int NAME = VALUE

namespace EVENT_ID {
    ENUM(GAME_START, 1000);
    ENUM(GAME_OVER, 1001);
    ENUM(GAME_WIN, 1002);
};

#endif //SDL2_UI_CONTEXT_H
