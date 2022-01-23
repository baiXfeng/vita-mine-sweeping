//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_TILE_H
#define SDL2_UI_TILE_H

class Tile {
public:
    Tile() {
        id = 0;
        hidden = true;
        is_mine = false;
        number = 0;
        pressed = false;
        flag = nullptr;
    }
    uint32_t id;
    bool hidden;        // 是否隐藏
    bool is_mine;       // 是否地雷
    char number;        // 数字提示
    bool pressed;       // 按下
    bool* flag;         // 标记小旗
};

#endif //SDL2_UI_TILE_H
