//
// Created by baifeng on 2022/1/24.
//

#ifndef SDL2_UI_OPTION_H
#define SDL2_UI_OPTION_H

#include "common/widget.h"
#include "ui-layout/ui-layout.h"

class Context;
class OptionView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutSelectorAssigner, public ui::LayoutNodeListener {
public:
    OptionView();
public:
    void setContext(Context& c);
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    Selector onResolveSelector(mge::Widget* target, const char* name) override;
    void onLayoutLoaded() override;
    void onButtonDown(int key) override;
    void onMapWidthChanged(mge::Widget* sender, int state);
    void onMapHeightChanged(mge::Widget* sender, int state);
    void onMineNumberChanged(mge::Widget* sender, int state);
    void onApply(mge::Widget* sender);
    void onTitle(mge::Widget* sender);
    void exitView();
private:
    enum Index {
        MAP_WIDTH = 0,
        MAP_HEIGHT = 1,
        MINE_NUMBER = 2,
    };
    void setBarValue(Index i, uint32_t value);
private:
    Context* ctx;
    mge::RenderTargetWidget* _renderTarget;
    mge::TTFLabel* _text[3];
    mge::ProgressBarWidget* _bar[3];
    uint32_t _valueCache[3];
};

class OptionViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(OptionView);
};

#endif //SDL2_UI_OPTION_H
