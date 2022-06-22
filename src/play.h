//
// Created by baifeng on 2022/1/23.
//

#ifndef SDL2_UI_PLAY_H
#define SDL2_UI_PLAY_H

#include "common/widget.h"
#include "common/gridmap.h"

class Context;
class Tile;
class PlayGame : public mge::LayerWidget, protected mge::GridMapDataSource {
public:
    PlayGame(Context& c);
public:
    mge::GridMapWidget* grid() const;
    void restart();
    void reload_data();
private:
    void onUpdate(float delta) override;
    bool onMouseDown(mge::MouseEvent const& event) override;
    void onMouseMotion(mge::MouseEvent const& event) override;
    void onMouseUp(mge::MouseEvent const& event) override;
private:
    size_t numberOfLayersInWidget(mge::GridMapWidget const* sender) override;
    mge::Vector2i sizeOfGridMap(mge::GridMapWidget const* sender) override;
    mge::Vector2i sizeOfGridTile(mge::GridMapWidget const* sender) override;
    Widget::Ptr tileWidgetAtPosition(mge::GridMapWidget* sender, int layerIndex, mge::Vector2i const& position) override;
private:
    Context* ctx;
    mge::GridMapWidget* _gridlayer;
    Tile* _tile;
    mge::Vector2i _prev;
    mge::Vector2f _move, _offset;
};

#endif //SDL2_UI_PLAY_H
