//
// Created by baifeng on 2022/1/23.
//

#include "play.h"
#include "context.h"
#include "tile.h"

using namespace mge;

class MineTileCell : public TileWidget {
public:
    MineTileCell() {
        SDL_Color color[3] = {
                {255, 0, 0, 255},
                {0, 255, 0, 255},
                {0, 0, 255, 255},
        };
        addChild(Ptr(new MaskWidget(color[rand() % 3])));
    }
    void setData(Tile* data) {
        _data = data;
    }
    void onModifySize(Vector2f const& size) override {
        _children[0]->setSize(size);
    }
private:
    Tile* _data;
};

PlayGame::PlayGame(Context& c):ctx(&c), _gridlayer(new GridMapWidget) {
    _gridlayer->setDataSource(this);
    addChild(Ptr(_gridlayer));
}

mge::GridMapWidget* PlayGame::grid() const {
    return _gridlayer;
}

size_t PlayGame::numberOfLayersInWidget(mge::GridMapWidget* sender) {
    return 1;
}

mge::Vector2i PlayGame::sizeOfGridMap(mge::GridMapWidget* sender) {
    return ctx->map.size();
}

mge::Vector2i PlayGame::sizeOfGridTile(mge::GridMapWidget* sender) {
    return {40, 40};
}

Widget::Ptr PlayGame::tileWidgetAtPosition(mge::GridMapWidget* sender, int layerIndex, mge::Vector2i const& position) {
    auto view = sender->dequeueTile(layerIndex);
    if (view == nullptr) {
        view.reset(new MineTileCell);
    }
    return view;
}