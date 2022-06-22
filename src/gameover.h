//
// Created by baifeng on 2022/1/24.
//

#ifndef SDL2_UI_GAMEOVER_H
#define SDL2_UI_GAMEOVER_H

#include "common/widget.h"
#include "ui-layout/ui-layout.h"

class GameOverView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener {
public:
    GameOverView();
public:
    void setTipsText(std::string const& text);
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    void onButtonDown(int key) override;
    bool onMouseDown(mge::MouseEvent const& event) override;
private:
    mge::TTFLabel* _tips;
};

class GameOverViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(GameOverView);
};

#endif //SDL2_UI_GAMEOVER_H
