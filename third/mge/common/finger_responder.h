//
// Created by baifeng on 2022/1/23.
//

#ifndef SDL2_UI_FINGER_RESPONDER_H
#define SDL2_UI_FINGER_RESPONDER_H

#include "macro.h"
#include "vector2.h"

mge_begin

class Widget;
class FingerResponder {
public:
    FingerResponder(Widget* widget):_owner(widget), _zorder(0) {}
    virtual ~FingerResponder() {}
    virtual bool onTouchBegen(Vector2i const& point) {
        return false;
    }
    virtual void onTouchMoved(Vector2i const& point) {}
    virtual void onTouchEnded(Vector2i const& point) {}
    Widget* owner() const {
        return _owner;
    }
    void setTouchZOrder(uint32_t zorder) {
        _zorder = zorder;
    }
    uint32_t getTouchZOrder() const {
        return _zorder;
    }
protected:
    Widget* _owner;
    uint32_t _zorder;
};

mge_end

#endif //SDL2_UI_FINGER_RESPONDER_H
