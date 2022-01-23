//
// Created by baifeng on 2021/10/4.
//

#ifndef SDL2_UI_MOUSE_H
#define SDL2_UI_MOUSE_H

#include <list>
#include <SDL.h>
#include "macro.h"
#include "vector2.h"

mge_begin

class FingerResponder;
class Mouse {
public:
    typedef std::list<FingerResponder*> List;
public:
    Mouse();
public:
    void sleep(float seconds);
    void add(FingerResponder* resp);
    void remove(FingerResponder* resp);
    void onEvent(SDL_Event const& event);
private:
    void onMouseEvent(SDL_Event const& event);
    void onFingerEvent(SDL_Event const& event);
private:
    void onFingerDown(Vector2i const& point);
    void onFingerUp(Vector2i const& point);
    void onFingerMotion(Vector2i const& point);
private:
    bool _sleep;
    bool _usefinger;
    bool _finger_downed;
    FingerResponder* _current;
    List _responder;
};

mge_end

#endif //SDL2_UI_MOUSE_H
