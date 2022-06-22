//
// Created by baifeng on 2021/10/4.
//

#include "mouse.h"
#include "game.h"
#include "widget.h"
#include "log.h"
#include "mouse_responder.h"
#include "action.h"
#include "defer_function.h"

mge_begin

Mouse::Mouse():_finger_downed(false), _usefinger(false), _current(nullptr), _focus(nullptr), _sleep(false), _sort_dirty(false), _zorder(0) {
#if defined(__vita__)
    _usefinger = true;
#endif
}

void Mouse::sleep(float seconds) {
    _sleep = true;
    auto delay = Action::Ptr(new Delay(seconds));
    auto call = Action::Ptr(new CallBackVoid([&]{
        this->_sleep = false;
    }));
    auto action = Action::Ptr(new Sequence({delay, call}));
    action->setName("Mouse::sleep");
    auto& widget = _game.screen();
    widget.stopAction(action->name());
    widget.runAction(action);
}

static uint32_t getWidgetZOrder(Widget* widget) {
    int ret = 1;
    while ((widget = widget->parent()) and ++ret) {}
    return ret;
}

void Mouse::sortResponderList() {
    if (!_sort_dirty) {
        return;
    }
    Defer<bool> defer([](bool* value){
        *value = false;
    }, &_sort_dirty);

    // 获取新注册的响应器
    _insertResponder.clear();
    for (auto& resp : _responder) {
        if (resp->getTouchZOrder() == 0) {
            _insertResponder.emplace(resp->owner(), resp);
        }
    }
    if (_insertResponder.empty()) {
        return;
    }

    // 为响应器设置层级权重
    uint32_t zorder = 0xffff;
    MouseResponder* responder = nullptr;
    for (auto& resp : _insertResponder) {
        if (auto value = getWidgetZOrder(resp.first); value < zorder) {
            zorder = value;
            responder = resp.second;
        }
    }
    visit_widget(responder->owner());

    // 对响应器排序
    _responder.sort([](MouseResponder const* first, MouseResponder const* second)->bool {
        return first->getTouchZOrder() < second->getTouchZOrder();
    });
}

void Mouse::visit_widget(Widget* widget) {
    if (auto iter = _insertResponder.find(widget); iter != _insertResponder.end()) {
        iter->second->setTouchZOrder(++_zorder);
    }
    for (auto& child : widget->children()) {
        visit_widget(child.get());
    }
}

MouseResponder* Mouse::getFocusResponder(Vector2i const& point) const {
    for (auto iter = _responder.rbegin(); iter != _responder.rend(); iter++) {
        if (auto resp = *iter; resp) {
            if (auto widget = resp->owner(); widget) {
                if (!widget->visible() or !widget->isTouchEnabled()) {
                    continue;
                }
                auto& position = widget->global_position();
                auto& size = widget->global_size();
                if (point.x > (int)position.x and point.x < int(position.x + size.x) and point.y > (int)position.y and point.y < int(position.y + size.y)) {
                    return resp;
                }
            }
        }
    }
    return nullptr;
}

void Mouse::add(MouseResponder* resp) {
    this->remove(resp);
    _responder.push_back(resp);
    _sort_dirty = true;

    const std::string name = "Mouse::sortResponderList";
    if (not _game.screen().hasAction(name)) {
        auto action = Action::New<CallBackVoid>(std::bind(&Mouse::sortResponderList, this));
        action->setName(name);
        _game.screen().runAction(action);
    }
}

void Mouse::remove(MouseResponder* resp) {
    if (auto iter = std::find(_responder.begin(), _responder.end(), resp); iter != _responder.end()) {
        _responder.erase(iter);
    }
    if (resp == _current) {
        _current = nullptr;
    }
    if (resp == _focus) {
        _focus = nullptr;
    }
    _sort_dirty = true;
}

void Mouse::onEvent(SDL_Event const& event) {
    _usefinger ? onFingerEvent(event) : onMouseEvent(event);
}

static std::map<int, Button> sdlMouseKeyMap = {
        {SDL_BUTTON_LEFT, MOUSE_LEFT},
        {SDL_BUTTON_MIDDLE, MOUSE_MIDDLE},
        {SDL_BUTTON_RIGHT, MOUSE_RIGHT},
};

void Mouse::onMouseEvent(SDL_Event const& event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
        {
            _finger_downed = true;
            auto button = sdlMouseKeyMap[event.button.button];
            onMouseDown({button, event.button.x, event.button.y});
        }
            break;
        case SDL_MOUSEBUTTONUP:
        {
            _finger_downed = false;
            auto button = sdlMouseKeyMap[event.button.button];
            onMouseUp({button, event.button.x, event.button.y});
        }
            break;
        case SDL_MOUSEMOTION:
            if (_finger_downed) {
                auto button = sdlMouseKeyMap[event.button.button];
                onMouseMotion({button, event.button.x, event.button.y});
            } else {
                // mouse enter/exit for desktop system
                auto current_focus = getFocusResponder({event.button.x, event.button.y});
                if (_focus == nullptr) {
                    if (current_focus) {
                        auto position = Vector2i{event.button.x, event.button.y} - current_focus->owner()->global_position().to<int>();
                        current_focus->onMouseEnter({MOUSE_NONE, position.x, position.y});
                        _focus = current_focus;
                    }
                } else if (_focus == current_focus) {
                    return;
                } else {
                    auto position = Vector2i{event.button.x, event.button.y} - _focus->owner()->global_position().to<int>();
                    _focus->onMouseExit({MOUSE_NONE, position.x, position.y});
                    _focus = nullptr;
                    if (current_focus) {
                        auto position = Vector2i{event.button.x, event.button.y} - current_focus->owner()->global_position().to<int>();
                        current_focus->onMouseEnter({MOUSE_NONE, position.x, position.y});
                        _focus = current_focus;
                    }
                }
            }
            break;
        case SDL_MOUSEWHEEL:
            if (_focus and _focus->onMouseWheel({MOUSE_NONE, event.wheel.x, event.wheel.y})) {
                return;
            } else {
                // 往下派发滚轮事件直到有响应器处理它
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
            auto& size = _game.screen().size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            auto point = (size * scale).to<int>();
            onMouseDown({MOUSE_NONE, point.x, point.y});
        }
            break;
        case SDL_FINGERUP:
        {
            auto& size = _game.screen().size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            auto point = (size * scale).to<int>();
            onMouseUp({MOUSE_NONE, point.x, point.y});
        }
            break;
        case SDL_FINGERMOTION:
        {
            auto& size = _game.screen().size();
            auto scale = Vector2f{event.tfinger.x, event.tfinger.y};
            auto point = (size * scale).to<int>();
            onMouseMotion({MOUSE_NONE, point.x, point.y});
        }
            break;
        default:
            break;
    }
}

void Mouse::onMouseDown(MouseEvent const& event) {
    //LOG("鼠标按下: %d, %d\n", point.x, point.y);
    if (_sleep) {
        _current = nullptr;
        return;
    }
    auto& point = event;
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
                    auto down_pos = Vector2i{point.x, point.y} - position.to<int>();
                    if (resp->onMouseDown({event.button, down_pos.x, down_pos.y})) {
                        if (_current = resp; resp->owner()->parent() == nullptr) {
                            _current = nullptr;
                        }
                        return;
                    }
                }
            }
        }
    }
    _current = nullptr;
}

void Mouse::onMouseUp(MouseEvent const& event) {
    //LOG("鼠标弹起: %d, %d\n", point.x, point.y);
    if (_current) {
        auto& point = event;
        if (auto widget = _current->owner(); widget) {
            auto position = Vector2i{point.x, point.y} - widget->global_position().to<int>();
            _current->onMouseUp({event.button, position.x, position.y});
        }
    }
}

void Mouse::onMouseMotion(MouseEvent const& event) {
    //LOG("鼠标移动: %d, %d\n", point.x, point.y);
    if (_current) {
        auto& point = event;
        if (auto widget = _current->owner(); widget) {
            auto position = Vector2i{point.x, point.y} - widget->global_position().to<int>();
            _current->onMouseMotion({event.button, position.x, position.y});
        }
    }
}

mge_end
