//
// Created by baifeng on 2022/1/24.
//

#include "gameover.h"

GameOverView::GameOverView():_tips(nullptr) {}

bool GameOverView::onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) {
    UI_LAYOUT_MEMBER_ASSIGN(this, "tips_text", mge::TTFLabel*, _tips);
    return false;
}

void GameOverView::setTipsText(std::string const& text) {
    _tips->setString(text);
}

void GameOverView::onButtonDown(int key) {
    removeFromParent();
}

bool GameOverView::onMouseDown(mge::MouseEvent const& event) {
    removeFromParent();
    return true;
}
