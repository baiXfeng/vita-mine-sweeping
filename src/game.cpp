//
// Created by baifeng on 2022/1/22.
//

#include "game.h"
#include "common/game.h"
#include "common/widget.h"
#include "play.h"

GameView::GameView():_time(nullptr), _mine(nullptr), _play(nullptr) {
    memset(_smile, 0, sizeof(_smile));
    _game.setRenderColor({18, 103, 72, 255});
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

void GameView::onOption(Widget* sender) {
    printf("设置\n");
    _play->restart();
}

void GameView::onRestart(Widget* sender) {
    printf("重新开始\n");
}

void GameView::onButtonDown(int key) {
    if (key == KeyCode::B) {
        _game.screen().pop();
    }
}
