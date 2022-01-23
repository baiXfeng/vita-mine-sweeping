//
// Created by baifeng on 2022/1/23.
//

#ifndef SDL2_UI_LOGIC_H
#define SDL2_UI_LOGIC_H

#include "common/vector2.h"

class Context;
bool restart_game(Context& c, mge::Vector2i const& mapSize, uint32_t number_of_mine);   // 重新开始游戏
bool click_tile(Context& c, mge::Vector2i const& position); // 点击一个块
bool open_tile(Context& c, mge::Vector2i const& position);  // 打开一个块
bool auto_clear(Context& c, mge::Vector2i const& position); // 根据数字自动清除周边空白块
bool clear_empty(Context& c, mge::Vector2i const& position);    // 清除连续的空白块
bool set_flag(Context& c, mge::Vector2i const& position);       // 标记为旗子
bool clear_all_mine(Context& c);                                // 结算所有地雷

#endif //SDL2_UI_LOGIC_H
