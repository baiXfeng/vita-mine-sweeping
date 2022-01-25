//
// Created by baifeng on 2022/1/24.
//

#include "option.h"
#include "common/action.h"
#include "common/game.h"
#include "context.h"

OptionView::OptionView():_renderTarget(nullptr), ctx(nullptr) {
    memset(_text, 0, sizeof(_text));
    memset(_bar, 0, sizeof(_bar));
    memset(_valueCache, 0, sizeof(_valueCache));

    connect(ON_ENTER, [this](mge::Widget* sender){
        ctx->state.paused = true;
    });
    connect(ON_EXIT, [this](mge::Widget* sender){
        ctx->state.paused = false;
    });
}

void OptionView::setContext(Context& c) {
    this->ctx = &c;
    _valueCache[MAP_WIDTH] = ctx->setting.map_width;
    _valueCache[MAP_HEIGHT] = ctx->setting.map_height;
    _valueCache[MINE_NUMBER] = ctx->setting.mine_number;
    for (int i = 0; i < 3; ++i) {
        setBarValue(Index(i), _valueCache[i]);
    }
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

OptionView::Selector OptionView::onResolveSelector(mge::Widget* target, const char* name) {
    UI_LAYOUT_SELECTOR_BIND(this, "onTitle", OptionView::onTitle);
    UI_LAYOUT_SELECTOR_BIND(this, "onApply", OptionView::onApply);
    return nullptr;
}

void OptionView::onLayoutLoaded() {
    mge::ProgressBarWidget::CallBack selector[3] = {
            std::bind(&OptionView::onMapWidthChanged, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OptionView::onMapHeightChanged, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&OptionView::onMineNumberChanged, this, std::placeholders::_1, std::placeholders::_2),
    };
    for (int i = 0; i < 3; ++i) {
        _bar[i]->setValue(0.5f);
        _bar[i]->setSelector(selector[i]);
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

void OptionView::onApply(mge::Widget* sender) {
    ctx->setting.map_width = _valueCache[MAP_WIDTH];
    ctx->setting.map_height = _valueCache[MAP_HEIGHT];
    ctx->setting.mine_number = _valueCache[MINE_NUMBER];
    exitView();
}

void OptionView::onTitle(mge::Widget* sender) {
    connect(ON_EXIT, [this](mge::Widget* sender){
        _game.screen().pop();
    });
    exitView();
}

void OptionView::onMapWidthChanged(mge::Widget* sender, int state) {
    auto value = sender->fast_to<mge::ProgressBarWidget>()->value();
    auto number = uint32_t(value * ctx->config.max_map_width);
    if (number <= ctx->config.min_map_width) {
        number = ctx->config.min_map_width;
    }
    setBarValue(MAP_WIDTH, number);
    onMineNumberChanged(sender, state);
}

void OptionView::onMapHeightChanged(mge::Widget* sender, int state) {
    auto value = sender->fast_to<mge::ProgressBarWidget>()->value();
    auto number = uint32_t(value * ctx->config.max_map_height);
    if (number <= ctx->config.min_map_height) {
        number = ctx->config.min_map_height;
    }
    setBarValue(MAP_HEIGHT, number);
    onMineNumberChanged(sender, state);
}

void OptionView::onMineNumberChanged(mge::Widget* sender, int state) {
    auto value = sender->fast_to<mge::ProgressBarWidget>()->value();
    auto max_mine_number = (_valueCache[MAP_WIDTH] * _valueCache[MAP_HEIGHT]) * 0.3f;
    auto min_mine_number = (_valueCache[MAP_WIDTH] * _valueCache[MAP_HEIGHT]) * 0.12f;
    auto number = uint32_t(value * max_mine_number);
    if (number <= min_mine_number) {
        number = min_mine_number;
    }
    setBarValue(MINE_NUMBER, number);
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

void OptionView::setBarValue(Index i, uint32_t value) {
    std::string text[3] = {
            "地图宽: %d",
            "地图高: %d",
            "地雷数量: %d",
    };
    char temp[80] = {0};
    sprintf(temp, text[i].c_str(), value);
    _text[i]->fast_to<mge::TTFLabel>()->setString(temp);
    _valueCache[i] = value;
}
