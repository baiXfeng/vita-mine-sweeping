//
// Created by baifeng on 2022/1/11.
//

#ifndef SDL2_UI_UI_LAYOUT_H
#define SDL2_UI_UI_LAYOUT_H

#include "layout-reader.h"
#include "node-loader.h"
#include "loader-pool.h"
#include "layout-variable-assigner.h"

/*
 * example code
 *
class MyWidget : public mge::WindowWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener {
public:
    MyWidget() {}
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override {
        UI_LAYOUT_MEMBER_ASSIGN(this, "imgIcon", mge::ImageWidget*, _icon);
        UI_LAYOUT_MEMBER_ASSIGN(this, "test2", mge::Widget*, _second);
        return false;
    }
    void onLayoutLoaded() override {}
private:
    mge::ImageWidget* _icon;
    mge::Widget* _second;
};

class MyWidgetLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(MyWidget);
};

*/

#endif //SDL2_UI_UI_LAYOUT_H
