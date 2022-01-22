//
// Created by baifeng on 2021/7/12.
//

#ifndef SDL2_UI_GAME_H
#define SDL2_UI_GAME_H

#include "common/game.h"
#include "common/log.h"
#include "common/command.h"
#include "common/widget.h"
#include "lutok3.h"
#include "uilayout/ui-layout.h"
#include "common/xml_layout.h"
#include <algorithm>

class MyWidget : public mge::WindowWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener {
public:
    MyWidget() {

    }
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override {
        UI_LAYOUT_MEMBER_ASSIGN(this, "imgIcon", mge::ImageWidget*, _icon);
        UI_LAYOUT_MEMBER_ASSIGN(this, "test2", mge::Widget*, _second);
        return false;
    }
    void onLayoutLoaded() override {

    }
private:
    mge::ImageWidget* _icon;
    mge::Widget* _second;
};

class MyWidgetLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(MyWidget);
};

class MyGame : public mge::Game::App {
public:
    MyGame() {}
    void init() override {
        LOG_INIT();
        //_game.screen().push<SurvivalView>();
    }
    void update(float delta) override {
        _game.screen().update(delta);
    }
    void render(SDL_Renderer* renderer) override {
        _game.screen().render(renderer);
    }
    void fini() override {
        _game.screen().popAll();
        LOG_FINI();
    }
};

#endif //SDL2_UI_GAME_H
