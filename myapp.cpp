//
// Created by baifeng on 2021/7/12.
//

#ifndef SDL2_UI_MYGAME_H
#define SDL2_UI_MYGAME_H

#include "common/game.h"
#include "common/log.h"
#include "common/widget.h"
#include "common/loadres.h"
#include "common/action.h"
#include "common/xml_layout.h"
#include "ui-layout/ui-layout.h"
#include "ui-layout/node-loader.h"
#include "src/start.h"
#include "src/game.h"
#include "src/play.h"
#include "src/gameover.h"
#include "src/option.h"

class MyApp : public mge::Game::App {
public:
    MyApp() {}
    void init() override {
        LOG_INIT();

        _game.uilayout().getLoaderPool()->addLoader<StartViewLoader>("StartView");
        _game.uilayout().getLoaderPool()->addLoader<GameViewLoader>("GameView");
        _game.uilayout().getLoaderPool()->addLoader<GameOverViewLoader>("GameOver");
        _game.uilayout().getLoaderPool()->addLoader<OptionViewLoader>("OptionView");

        /*
        auto texture = mge::res::load_texture("assets/images/title-bg.png");
        _game.screen().push<mge::ImageWidget>(texture);
        _game.screen().scene_back()->defer([]{
            _game.screen().replace( _game.uilayout().readNode("assets/layouts/start.xml") );
        }, 0.02f);
         */

        _game.screen().push( _game.uilayout().readNode("assets/layouts/test.xml") );
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

Startup(MyApp);

#endif //SDL2_UI_MYGAME_H
