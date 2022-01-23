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
    FingerResponder(Widget* widget);
    virtual ~FingerResponder() {}
    virtual bool onTouchBegen(Vector2i const& point) {
        return false;
    }
    virtual void onTouchEnded(Vector2i const& point) {}
    virtual void onTouchMoved(Vector2i const& point) {}
    virtual uint32_t getLayerIndex() const;
    bool operator > (FingerResponder& f) {
        return getLayerIndex() > f.getLayerIndex();
    }
    bool operator < (FingerResponder& f) {
        return getLayerIndex() < f.getLayerIndex();
    }
    Widget* owner() const;
protected:
    Widget* _owner;
};

mge_end

#endif //SDL2_UI_FINGER_RESPONDER_H
