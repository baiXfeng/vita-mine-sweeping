//
// Created by baifeng on 2022/1/23.
//

#ifndef SDL2_UI_PLAY_H
#define SDL2_UI_PLAY_H

#include "common/widget.h"
#include "common/gridmap.h"

class Context;
class PlayGame : public mge::WindowWidget, protected mge::GridMapDataSource {
public:
    PlayGame(Context& c);
public:
    mge::GridMapWidget* grid() const;
private:
    size_t numberOfLayersInWidget(mge::GridMapWidget* sender) override;
    mge::Vector2i sizeOfGridMap(mge::GridMapWidget* sender) override;
    mge::Vector2i sizeOfGridTile(mge::GridMapWidget* sender) override;
    Widget::Ptr tileWidgetAtPosition(mge::GridMapWidget* sender, int layerIndex, mge::Vector2i const& position) override;
private:
    Context* ctx;
    mge::GridMapWidget* _gridlayer;
};

#endif //SDL2_UI_PLAY_H
