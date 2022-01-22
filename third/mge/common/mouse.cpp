//
// Created by baifeng on 2021/10/4.
//

#include "mouse.h"
#include "game.h"
#include "widget.h"
#include "log.h"

mge_begin

Mouse::Mouse():_finger_downed(false), _usefinger(false) {
#if defined(__vita__)
    _usefinger = true;
#endif
}

void Mouse::sleep(float seconds) {

}

void Mouse::add(WidgetPtr const& widget) {

}

void Mouse::remove(WidgetPtr const& widget) {

}

void Mouse::remove(Widget const* widget) {

}

void Mouse::onEvent(SDL_Event const& event) {
    _usefinger ? onFingerEvent(event) : onMouseEvent(event);
}

void Mouse::onMouseEvent(SDL_Event const& event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            _finger_downed = true;
            onFingerDown({event.button.x, event.button.y});
            break;
        case SDL_MOUSEBUTTONUP:
            _finger_downed = false;
            onFingerUp({event.button.x, event.button.y});
            break;
        case SDL_MOUSEMOTION:
            if (_finger_downed) {
                onFingerMotion({event.button.x, event.button.y});
            }
            break;
        default:
            break;
    }
}

void Mouse::onFingerEvent(SDL_Event const& event) {
    if (event.tfinger.touchId != 0) {
        // 排除背面触摸
        return;
    }
    switch (event.type) {
        case SDL_FINGERDOWN:
        {
            auto& size = _game.screen().screen_size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            onFingerDown((size * scale).to<int>());
        }
            break;
        case SDL_FINGERUP:
        {
            auto& size = _game.screen().screen_size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            onFingerUp((size * scale).to<int>());
        }
            break;
        case SDL_FINGERMOTION:
        {
            auto& size = _game.screen().screen_size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            onFingerMotion((size * scale).to<int>());
        }
            break;
        default:
            break;
    }
}

void Mouse::onFingerDown(Vector2i const& point) {
    LOG("鼠标按下: %d, %d\n", point.x, point.y);
}

void Mouse::onFingerUp(Vector2i const& point) {
    LOG("鼠标弹起: %d, %d\n", point.x, point.y);
}

void Mouse::onFingerMotion(Vector2i const& point) {
    LOG("鼠标移动: %d, %d\n", point.x, point.y);
}

mge_end
