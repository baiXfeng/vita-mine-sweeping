//
// Created by baifeng on 2022/1/23.
//

#ifndef SDL2_UI_MOUSE_RESPONDER_H
#define SDL2_UI_MOUSE_RESPONDER_H

#include "macro.h"
#include "vector2.h"
#include "mouse_event.h"

mge_begin

class Widget;
class MouseResponder {
public:
    MouseResponder(Widget* widget): _owner(widget), _zorder(0), _focus(false) {}
    virtual ~MouseResponder() {}
    virtual bool onMouseDown(MouseEvent const& event) {
        return false;
    }
    virtual void onMouseMotion(MouseEvent const& event) {}
    virtual void onMouseUp(MouseEvent const& event) {}
    virtual bool onMouseWheel(MouseEvent const& event) {
        return false;
    }
    virtual void onMouseEnter(MouseEvent const& event) {
        _focus = true;
    }
    virtual void onMouseExit(MouseEvent const& event) {
        _focus = false;
    }
public:
    Widget* owner() const {
        return _owner;
    }
    void setTouchZOrder(uint32_t zorder) {
        _zorder = zorder;
    }
    uint32_t getTouchZOrder() const {
        return _zorder;
    }
    bool focus() const {
        return _focus;
    }
protected:
    bool _focus;
    Widget* _owner;
    uint32_t _zorder;
};

mge_end

#endif //SDL2_UI_MOUSE_RESPONDER_H
