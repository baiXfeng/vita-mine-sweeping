//
// Created by baifeng on 2022/1/24.
//

#include "option.h"
#include "common/action.h"
#include "context.h"

OptionView::OptionView():_renderTarget(nullptr), ctx(nullptr) {
    memset(_text, 0, sizeof(_text));
    memset(_bar, 0, sizeof(_bar));

    connect(ON_ENTER, [this](mge::Widget* sender){
        ctx->paused = true;
    });
    connect(ON_EXIT, [this](mge::Widget* sender){
        ctx->paused = false;
    });
}

void OptionView::setContext(Context& c) {
    this->ctx = &c;
}

bool OptionView::onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) {
    UI_LAYOUT_MEMBER_ASSIGN(this, "bar1", mge::ProgressBarWidget*, _bar[0]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "bar2", mge::ProgressBarWidget*, _bar[1]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "bar3", mge::ProgressBarWidget*, _bar[2]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "map_width", mge::TTFLabel*, _text[0]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "map_height", mge::TTFLabel*, _text[1]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "mine_number", mge::TTFLabel*, _text[2]);
    UI_LAYOUT_MEMBER_ASSIGN(this, "render_target", mge::RenderTargetWidget*, _renderTarget);
    return false;
}

void OptionView::onLayoutLoaded() {
    for (int i = 0; i < 3; ++i) {
        _bar[i]->setSelector(std::bind(&OptionView::onProgress, this, std::placeholders::_1, std::placeholders::_2));
    }
    performLayout();
    _renderTarget->setRenderTargetSize(size().to<int>());
    auto scale = mge::Action::New<mge::ScaleTo>(_renderTarget, mge::Vector2f{1.0f, 1.0f}, 0.3f);
    auto finished = mge::Action::New<mge::CallBackVoid>([this]{
        _renderTarget->setRenderTargetNull();
    });
    auto action = mge::Action::Ptr(new mge::Sequence({scale, finished}));
    runAction(action);
    _renderTarget->setScale(0.01f);
}

void OptionView::onButtonDown(int key) {
    if (key == KeyCode::B) {
        exitView();
    }
}

void OptionView::onProgress(mge::Widget* sender, int state) {
    std::string text[3] = {
            "地图宽: %d",
            "地图高: %d",
            "地雷数量: %d",
    };
    for (int i = 0; i < 3; ++i) {
        if (sender == _bar[i]) {
            auto bar = _bar[i]->fast_to<mge::ProgressBarWidget>();
            char temp[80] = {0};
            sprintf(temp, text[i].c_str(), int(bar->value() * 100));
            _text[i]->fast_to<mge::TTFLabel>()->setString(temp);
        }
    }
}

void OptionView::exitView() {
    _renderTarget->setRenderTargetSize(size().to<int>());
    auto scale = mge::Action::New<mge::ScaleTo>(_renderTarget, mge::Vector2f{0.01f, 0.01f}, 0.3f);
    auto finished = mge::Action::New<mge::CallBackVoid>([this]{
        _renderTarget->setRenderTargetNull();
        removeFromParent();
    });
    auto action = mge::Action::Ptr(new mge::Sequence({scale, finished}));
    runAction(action);
    _renderTarget->setScale(1.0f);
}
