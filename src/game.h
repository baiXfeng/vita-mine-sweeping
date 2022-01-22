//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_GAME_H
#define SDL2_UI_GAME_H

#include "common/widget.h"
#include "uilayout/ui-layout.h"

class GameView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener {
public:
    GameView();
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    void onLayoutLoaded() override;
    void onButtonDown(int key) override;
private:
};

class GameViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(GameView);
};

#endif //SDL2_UI_GAME_H
