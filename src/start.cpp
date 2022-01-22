//
// Created by baifeng on 2022/1/22.
//

#include "start.h"
#include "common/action.h"
#include "common/game.h"

using namespace mge;

StartView::StartView():_tips(nullptr) {

}

bool StartView::onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) {
    UI_LAYOUT_MEMBER_ASSIGN(this, "tips_text", mge::TTFLabel*, _tips);
    return false;
}

void StartView::onLayoutLoaded() {
    auto blink = Action::Ptr(new Blink(_tips, 2, 1.5f));
    auto action = Action::New<Repeat>(blink);
    action->setName("blink");
    runAction(action);
}

void StartView::onButtonDown(int key) {
    sleep_gamepad(10.0f);
    auto blink = Action::Ptr(new Blink(_tips, 5, 1.0f));
    auto call = Action::New<CallBackVoid>([this](){
        _tips->setVisible(true);
        sleep_gamepad(0.0f);
        this->gotoGame();
    });
    _tips->setVisible(true);
    stopAction("blink");
    runAction(Action::Ptr(new Sequence({blink, call})));
}

void StartView::gotoGame() {
    _game.screen().pop();
}
