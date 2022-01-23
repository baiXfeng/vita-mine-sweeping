//
// Created by baifeng on 2022/1/23.
//

#include "play.h"
#include "context.h"
#include "tile.h"
#include "logic.h"
#include "common/texture.h"
#include "common/loadres.h"

using namespace mge;

class AutoSetTexture {
public:
    AutoSetTexture(ImageWidget* image):_img(image) {}
    ~AutoSetTexture() {
        _img->setTexture(_texture);
        _img->setSize(_img->parent()->size());
    }
    void setTexture(Texture::Ptr const& texture) {
        _texture = texture;
    }
private:
    ImageWidget* _img;
    Texture::Ptr _texture;
};

class MineTileCell : public TileWidget {
public:
    MineTileCell():_icon(nullptr) {
        addChild(Ptr(_icon = new ImageWidget));
        _base = res::load_texture("assets/images/tile_base.png");
        _flag = res::load_texture("assets/images/tile_f.png");
        _flag_on = res::load_texture("assets/images/tile_d.png");
        _mine = res::load_texture("assets/images/tile_b2.png");
        _bomb = res::load_texture("assets/images/tile_b.png");
        _wrong = res::load_texture("assets/images/tile_b3.png");
        for (int i = 0; i < 8; ++i) {
            char temp[80] = {0};
            sprintf(temp, "assets/images/tile_%d.png", i+1);
            _number[i] = res::load_texture(temp);
        }
        _btn[0] = res::load_texture("assets/images/tile_mask.png");
        _btn[1] = res::load_texture("assets/images/tile_down.png");
        enableUpdate(true);
    }
    void setData(Tile* data) {
        _data = data;
    }
    void onUpdate(float delta) override {
        AutoSetTexture set(_icon);
        auto texture = _base;
        set.setTexture(texture);
        if (_data->hidden) {
            if (_data->flag_wrong) {
                set.setTexture(_wrong);
                return;
            } else if (_data->flag_on) {
                set.setTexture(_flag_on);
                return;
            } else if (_data->flag and *_data->flag) {
                set.setTexture(_flag);
                return;
            }
            set.setTexture(_btn[_data->pressed ? 1 : 0]);
            return;
        }
        if (_data->number >= 1) {
            set.setTexture(_number[ _data->number - 1 ]);
        } else if (_data->is_mine) {
            if (_data->bomb) {
                set.setTexture(_bomb);
                return;
            }
            set.setTexture(_mine);
        }
    }
private:
    Tile* _data;
    ImageWidget* _icon;
    Texture::Ptr _base;
    Texture::Ptr _mine;
    Texture::Ptr _number[8];
    Texture::Ptr _btn[2];
    Texture::Ptr _flag;
    Texture::Ptr _flag_on;
    Texture::Ptr _bomb;
    Texture::Ptr _wrong;
};

#define TILE_SIZE 40

PlayGame::PlayGame(Context& c):ctx(&c), _gridlayer(new GridMapWidget), _tile(nullptr) {
    _gridlayer->setDataSource(this);
    addChild(Ptr(_gridlayer));
    restart();
}

bool PlayGame::onTouchBegen(mge::Vector2i const& point) {
    auto position = point + _gridlayer->getCamera()->getCameraPosition().to<int>();
    auto size = _gridlayer->getLayer(0)->size().to<int>();
    RectI bound{0, 0, size.x, size.y};
    if (!bound.contain(position)) {
        return false;
    }
    int x = position.x / TILE_SIZE;
    int y = position.y / TILE_SIZE;
    if (ctx->grid.is_out_of_range(x, y)) {
        return false;
    }
    auto& cell = ctx->grid.get(x, y);
    cell.pressed = true;
    _tile = &cell;
    return true;
}

void PlayGame::onTouchEnded(mge::Vector2i const& point) {
    if (_tile) {
        _tile->pressed = false;
        _tile = nullptr;

        auto position = point + _gridlayer->getCamera()->getCameraPosition().to<int>();
        int x = position.x / TILE_SIZE;
        int y = position.y / TILE_SIZE;

        if (ctx->grid.is_out_of_range(x, y)) {
            return;
        }

        click_tile(*ctx, {x, y});
    }
}

void PlayGame::onTouchMoved(mge::Vector2i const& point) {
    if (_tile == nullptr) {
        return;
    }
    auto position = point + _gridlayer->getCamera()->getCameraPosition().to<int>();
    int x = position.x / TILE_SIZE;
    int y = position.y / TILE_SIZE;
    if (ctx->grid.is_out_of_range(x, y)) {
        _tile->pressed = false;
        _tile = nullptr;
        return;
    }
    auto& cell = ctx->grid.get(x, y);
    if (cell.id != _tile->id) {
        _tile->pressed = false;
        _tile = nullptr;
    }
}

mge::GridMapWidget* PlayGame::grid() const {
    return _gridlayer;
}

void PlayGame::restart() {
    restart_game(*ctx, {10, 10}, 5);
    _gridlayer->reload_data();
}

size_t PlayGame::numberOfLayersInWidget(mge::GridMapWidget* sender) {
    return 1;
}

mge::Vector2i PlayGame::sizeOfGridMap(mge::GridMapWidget* sender) {
    return ctx->grid.size();
}

mge::Vector2i PlayGame::sizeOfGridTile(mge::GridMapWidget* sender) {
    return {TILE_SIZE, TILE_SIZE};
}

Widget::Ptr PlayGame::tileWidgetAtPosition(mge::GridMapWidget* sender, int layerIndex, mge::Vector2i const& position) {
    auto view = sender->dequeueTile(layerIndex);
    if (view == nullptr) {
        view.reset(new MineTileCell);
        view->fast_to<MineTileCell>()->setData(&ctx->grid.get(position));
    }
    return view;
}