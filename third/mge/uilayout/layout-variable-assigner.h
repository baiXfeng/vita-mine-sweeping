//
// Created by baifeng on 2022/1/12.
//

#ifndef SDL2_UI_LAYOUT_VARIABLE_ASSIGNER_H
#define SDL2_UI_LAYOUT_VARIABLE_ASSIGNER_H

#include <string.h>
#include <assert.h>
#include <functional>

#define UI_LAYOUT_MEMBER_ASSIGN(TARGET, NAME, TYPE, MEMBER) \
    if (target == TARGET && strcmp(NAME, name) == 0) {      \
        MEMBER = dynamic_cast<TYPE>(node);                  \
        assert(MEMBER && "LayoutVariableAssigner::onAssignMember fail."); \
        return true;                                                        \
    }

#define UI_LAYOUT_SELECTOR_BIND(TARGET, SELECTORNAME, METHOD)   \
    if (target == TARGET && strcmp(SELECTORNAME, name) == 0) {  \
        return std::bind(&METHOD, TARGET, std::placeholders::_1); \
    }

namespace mge {
    class Widget;
}

namespace ui {

    class LayoutVariableAssigner {
    public:
        virtual ~LayoutVariableAssigner() {}
        virtual bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) = 0;
        virtual bool onAssignProperty(mge::Widget* target, const char* name, const char* value) {
            return false;
        }
    };

    class LayoutSelectorAssigner {
    public:
        typedef std::function<void(mge::Widget*)> Selector;
    public:
        virtual ~LayoutSelectorAssigner() {}
        virtual Selector onResolveSelector(mge::Widget* target, const char* name) {
            return nullptr;
        }
    };

    class LayoutNodeListener {
    public:
        virtual ~LayoutNodeListener() {}
        virtual void onLayoutLoaded() {}
    };
}

#endif //SDL2_UI_LAYOUT_VARIABLE_ASSIGNER_H
