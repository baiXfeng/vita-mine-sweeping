//
// Created by baifeng on 2022/1/22.
//

#include "game.h"
#include "common/game.h"
#include "common/widget.h"
#include "common/xml_layout.h"
#include "play.h"
#include "gameover.h"

GameView::GameView():_time(nullptr), _mine(nullptr), _play(nullptr) {
    memset(_smile, 0, sizeof(_smile));
    _game.setRenderColor({18, 103, 72, 255});

    connect(ON_ENTER, [this](mge::Widget* sender){
        _game.event().add(EVENT_ID::GAME_OVER, this);
        _game.event().add(EVENT_ID::GAME_START, this);
        _game.event().add(EVENT_ID::GAME_WIN, this);
    });
    connect(ON_EXIT, [this](mge::Widget* sender){
        _game.event().remove(EVENT_ID::GAME_OVER, this);
        _game.event().remove(EVENT_ID::GAME_START, this);
        _game.event().remove(EVENT_ID::GAME_WIN, this);
    });
    _c.add(this);
}

bool GameView::onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) {
    UI_LAYOUT_MEMBER_ASSIGN(this, "time_text", mge::TTFLabel*, _time);
    UI_LAYOUT_MEMBER_ASSIGN(this, "mine_text", mge::TTFLabel*, _mine);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile1", mge::ButtonWidget*, _smile[0]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile2", mge::ButtonWidget*, _smile[1]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile3", mge::ButtonWidget*, _smile[2]);
    return false;
}

GameView::Selector GameView::onResolveSelector(mge::Widget* target, const char* name) {
    UI_LAYOUT_SELECTOR_BIND(this, "onOption", GameView::onOption);
    UI_LAYOUT_SELECTOR_BIND(this, "onRestart1", GameView::onRestart);
    UI_LAYOUT_SELECTOR_BIND(this, "onRestart2", GameView::onRestart);
    UI_LAYOUT_SELECTOR_BIND(this, "onRestart3", GameView::onRestart);
    return nullptr;
}

void GameView::onLayoutLoaded() {
    addChild(Ptr(_play = new PlayGame(_c)), 0);
    auto camera = _play->grid()->getCamera();
    auto layer_size = _play->grid()->getLayer(0)->size();
    camera->setCameraPosition((layer_size - size()) * 0.5f);
}

void GameView::onUpdate(float delta) {
    if (!_c.finished) {
        _c.seconds += delta;
        _c.notify(&ContextObserver::onTimeModify, _c.seconds);
    }
}

void GameView::onOption(Widget* sender) {

}

void GameView::onRestart(Widget* sender) {
    _play->restart();
}

void GameView::onButtonDown(int key) {
    if (key == KeyCode::B) {
        _game.screen().pop();
    } else if (key == KeyCode::L1 or key == KeyCode::R1) {
        _c.flag = !_c.flag;
    }
}

void GameView::onEvent(mge::Event const& e) {
    if (e.Id() == EVENT_ID::GAME_OVER) {
        this->setFaceState(1);
        this->addGameOver("GAME OVER!");
        _play->setTouchEnable(false);
    } else if (e.Id() == EVENT_ID::GAME_START) {
        this->setFaceState(0);
        _play->setTouchEnable(true);
    } else if (e.Id() == EVENT_ID::GAME_WIN) {
        this->setFaceState(2);
        this->addGameOver("YOU WIN!");
        _play->setTouchEnable(false);
    }
}

void GameView::onTimeModify(float seconds) {
    uint32_t value = int(seconds);
    char temp[80] = {0};
    sprintf(temp, "%02d:%02d", int(value / 60), value % 60);
    _time->setString(temp);
}

void GameView::onMineNumberModify(uint32_t number) {
    _mine->setString(std::to_string(number));
}

void GameView::setFaceState(int state) {
    for (int i = 0; i < 3; ++i) {
        _smile[i]->setVisible(state == i ? true : false);
    }
}

void GameView::addGameOver(std::string const& tips) {
    auto node = _game.uilayout().readNode("assets/layouts/gameover.xml");
    node->fast_to<GameOverView>()->setTipsText(tips);
    addChild(node);
}
