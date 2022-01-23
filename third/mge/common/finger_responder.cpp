//
// Created by baifeng on 2022/1/23.
//

#include "finger_responder.h"
#include "widget.h"

mge_begin

FingerResponder::FingerResponder(Widget* widget):_owner(widget) {}

uint32_t FingerResponder::getLayerIndex() const {
    if (auto widget = _owner; widget) {
        auto ret = 0;
        while ((widget = widget->parent()) and ++ret) {}
        return ret;
    }
    return 0;
}

Widget* FingerResponder::owner() const {
    return _owner;
}

mge_end