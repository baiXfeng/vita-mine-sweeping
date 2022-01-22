//
// Created by baifeng on 2022/1/22.
//

#ifndef SDL2_UI_GAME_H
#define SDL2_UI_GAME_H

#include "common/widget.h"
#include "uilayout/ui-layout.h"
#include "context.h"

class PlayGame;
class GameView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutSelectorAssigner, public ui::LayoutNodeListener {
public:
    GameView();
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    Selector onResolveSelector(mge::Widget* target, const char* name) override;
    void onLayoutLoaded() override;
    void onButtonDown(int key) override;
    void onOption(Widget* sender);
    void onRestart(Widget* sender);
private:
    Context _c;
    mge::ButtonWidget* _smile[3];
    mge::TTFLabel* _time;
    mge::TTFLabel* _mine;
    PlayGame* _play;
};

class GameViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(GameView);
};

#endif //SDL2_UI_GAME_H
