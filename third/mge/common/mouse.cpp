//
// Created by baifeng on 2021/10/4.
//

#include "mouse.h"
#include "game.h"
#include "log.h"

mge_begin

Mouse::Mouse():_finger_downed(false) {

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
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            //printf("鼠标按下: %d, %d\n", event.button.x, event.button.y);
            _finger_downed = true;
            onFingerEvent(FINGER_DOWN, {event.button.x, event.button.y});
            break;
        case SDL_MOUSEBUTTONUP:
            //printf("鼠标弹起: %d, %d\n", event.button.x, event.button.y);
            _finger_downed = false;
            onFingerEvent(FINGER_UP, {event.button.x, event.button.y});
            break;
        case SDL_MOUSEMOTION:
            if (_finger_downed) {
                //printf("鼠标移动: %d, %d\n", event.button.x, event.button.y);
                onFingerEvent(FINGER_MOTION, {event.button.x, event.button.y});
            }
            break;
        default:
            break;
    }
}

void Mouse::onFingerEvent(Event e, Vector2i const& postion) {
    switch (e) {
        case FINGER_DOWN:
            onFingerDown(postion);
            break;
        case FINGER_UP:
            onFingerUp(postion);
            break;
        case FINGER_MOTION:
            onFingerMotion(postion);
            break;
        default:
            break;
    }
}

void Mouse::onFingerDown(Vector2i const& postion) {

}

void Mouse::onFingerUp(Vector2i const& postion) {

}

void Mouse::onFingerMotion(Vector2i const& postion) {

}

mge_end
