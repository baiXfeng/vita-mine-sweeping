//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_START_H
#define SDL2_UI_START_H

#include "common/widget.h"
#include "uilayout/ui-layout.h"

class StartView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener, public mge::ScreenWidgetListener {
public:
    StartView();
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    void onLayoutLoaded() override;
    void onButtonDown(int key) override;
    bool onTouchBegen(mge::Vector2i const& point) override;
    void gotoGame();
private:
    void onScreenWake() override;
private:
    mge::TTFLabel* _tips;
};

class StartViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(StartView);
};

#endif //SDL2_UI_START_H
