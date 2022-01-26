//
// Created by baifeng on 2021/10/4.
//

#ifndef SDL2_UI_MOUSE_H
#define SDL2_UI_MOUSE_H

#include <list>
#include <map>
#include <SDL.h>
#include "macro.h"
#include "vector2.h"

mge_begin

class Widget;
class FingerResponder;
class Mouse {
    friend class Game;
public:
    typedef std::list<FingerResponder*> List;
public:
    Mouse();
public:
    void sleep(float seconds);
    void add(FingerResponder* resp);
    void remove(FingerResponder* resp);
private:
    void onEvent(SDL_Event const& event);
    void onMouseEvent(SDL_Event const& event);
    void onFingerEvent(SDL_Event const& event);
private:
    void onFingerDown(Vector2i const& point);
    void onFingerUp(Vector2i const& point);
    void onFingerMotion(Vector2i const& point);
private:
    void sortResponderList();
    void visit_widget(Widget* widget);
private:
    bool _sleep;
    bool _usefinger;
    bool _finger_downed;
    bool _sort_dirty;
    uint32_t _zorder;
    FingerResponder* _current;
    List _responder;
private:
    typedef std::map<Widget*, FingerResponder*> ResponderMap;
    ResponderMap _insertResponder;
};

mge_end

#endif //SDL2_UI_MOUSE_H
