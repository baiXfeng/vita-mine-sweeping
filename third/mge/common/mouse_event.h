//
// Created by baifeng on 2022/4/21.
//

#ifndef SDL2_UI_MOUSE_EVENT_H
#define SDL2_UI_MOUSE_EVENT_H

#include "macro.h"

mge_begin

    enum Button {
        MOUSE_NONE = 0,
        MOUSE_LEFT,
        MOUSE_MIDDLE,
        MOUSE_RIGHT,
        MOUSE_WHEEL,
    };
    struct MouseEvent {
        Button button;
        int32_t x, y;
    };

mge_end

#endif //SDL2_UI_MOUSE_EVENT_H
