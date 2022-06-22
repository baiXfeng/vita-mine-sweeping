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
#include "mouse_event.h"

mge_begin

class Widget;
class MouseResponder;
class Mouse {
    friend class Game;
public:
    typedef std::list<MouseResponder*> List;
public:
    Mouse();
public:
    void sleep(float seconds);
    void add(MouseResponder* resp);
    void remove(MouseResponder* resp);
private:
    void onEvent(SDL_Event const& event);
    void onMouseEvent(SDL_Event const& event);
    void onFingerEvent(SDL_Event const& event);
private:
    void onMouseDown(MouseEvent const& event);
    void onMouseUp(MouseEvent const& event);
    void onMouseMotion(MouseEvent const& event);
private:
    void sortResponderList();
    void visit_widget(Widget* widget);
    MouseResponder* getFocusResponder(Vector2i const& pos) const;
private:
    bool _sleep;
    bool _usefinger;
    bool _finger_downed;
    bool _sort_dirty;
    uint32_t _zorder;
    MouseResponder* _current;
    MouseResponder* _focus;
    List _responder;
private:
    typedef std::map<Widget*, MouseResponder*> ResponderMap;
    ResponderMap _insertResponder;
};

mge_end

#endif //SDL2_UI_MOUSE_H
