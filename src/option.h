//
// Created by baifeng on 2022/1/24.
//

#ifndef SDL2_UI_OPTION_H
#define SDL2_UI_OPTION_H

#include "common/widget.h"
#include "uilayout/ui-layout.h"

class Context;
class OptionView : public mge::GamePadWidget, public ui::LayoutVariableAssigner, public ui::LayoutNodeListener {
public:
    OptionView();
public:
    void setContext(Context& c);
private:
    bool onAssignMember(mge::Widget* target, const char* name, mge::Widget* node) override;
    void onLayoutLoaded() override;
    void onButtonDown(int key) override;
    void onProgress(mge::Widget* sender, int state);
    void exitView();
private:
    Context* ctx;
    mge::RenderTargetWidget* _renderTarget;
    mge::TTFLabel* _text[3];
    mge::ProgressBarWidget* _bar[3];
};

class OptionViewLoader : public ui::NodeLoader {
    UI_NODE_LOADER_CREATE(OptionView);
};

#endif //SDL2_UI_OPTION_H
