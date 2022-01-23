//
// Created by baifeng on 2021/10/4.
//

#include "mouse.h"
#include "game.h"
#include "widget.h"
#include "log.h"
#include "finger_responder.h"

mge_begin

Mouse::Mouse():_finger_downed(false), _usefinger(false), _current(nullptr) {
#if defined(__vita__)
    _usefinger = true;
#endif
}

void Mouse::sleep(float seconds) {

}

static uint32_t getLevel(Widget* widget) {
    int ret = 1;
    while ((widget = widget->parent()) and ++ret) {}
    return ret;
}

void Mouse::add(FingerResponder* resp) {
    this->remove(resp);
    _responder.push_back(resp);
    _responder.sort();
}

void Mouse::remove(FingerResponder* resp) {
    if (auto iter = std::find(_responder.begin(), _responder.end(), resp); iter != _responder.end()) {
        _responder.erase(iter);
    }
    if (resp == _current) {
        _current = nullptr;
    }
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
            _finger_downed ? onFingerMotion({event.button.x, event.button.y}) : void();
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
    //LOG("鼠标按下: %d, %d\n", point.x, point.y);
    for (auto iter = _responder.rbegin(); iter != _responder.rend(); iter++) {
        if (auto resp = *iter; resp) {
            if (auto widget = resp->owner(); widget) {
                if (!widget->visible() or !widget->isTouchEnabled()) {
                    continue;
                }
                auto& position = widget->global_position();
                auto& size = widget->global_size();
                if (point.x > (int)position.x and point.x < int(position.x + size.x) and
                    point.y > (int)position.y and point.y < int(position.y + size.y)) {
                    if (resp->onTouchBegen(point - position.to<int>())) {
                        _current = resp;
                        return;
                    }
                }
            }
        }
    }
    _current = nullptr;
}

void Mouse::onFingerUp(Vector2i const& point) {
    //LOG("鼠标弹起: %d, %d\n", point.x, point.y);
    if (_current) {
        if (auto widget = _current->owner(); widget) {
            _current->onTouchEnded(point - widget->global_position().to<int>());
        }
    }
}

void Mouse::onFingerMotion(Vector2i const& point) {
    //LOG("鼠标移动: %d, %d\n", point.x, point.y);
    if (_current) {
        if (auto widget = _current->owner(); widget) {
            _current->onTouchMoved(point - widget->global_position().to<int>());
        }
    }
}

mge_end
