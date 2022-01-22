//
// Created by baifeng on 2022/1/22.
//

#include "game.h"
#include "common/game.h"
#include "common/widget.h"

GameView::GameView():_time(nullptr), _mine(nullptr) {
    memset(_smile, 0, sizeof(_smile));
    _game.setRenderColor({55, 55, 55, 255});
}

bool GameView::onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) {
    UI_LAYOUT_MEMBER_ASSIGN(this, "time_text", mge::TTFLabel*, _time);
    UI_LAYOUT_MEMBER_ASSIGN(this, "mine_text", mge::TTFLabel*, _mine);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile1", mge::ButtonWidget*, _smile[0]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile2", mge::ButtonWidget*, _smile[1]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "smile3", mge::ButtonWidget*, _smile[2]);
    return false;
}

void GameView::onLayoutLoaded() {

}

void GameView::onButtonDown(int key) {

}
