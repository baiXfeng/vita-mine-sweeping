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

#define TILE_SIZE 56

PlayGame::PlayGame(Context& c):ctx(&c), _gridlayer(new GridMapWidget), _tile(nullptr) {
    _gridlayer->setDataSource(this);
    addChild(Ptr(_gridlayer));
    _gridlayer->getCamera()->setPadding({100, 150, 100, 150});
}

void PlayGame::onUpdate(float delta) {
    _gridlayer->getCamera()->move_offset(_move);
    _move *= 0.98f;
}

bool PlayGame::onTouchBegen(mge::Vector2i const& point) {

    _move.reset();

    auto world_pos = covertToWorldPosition(point.to<float>());
    auto position = _gridlayer->getLayer(0)->covertToLocalPosition(world_pos);

    int x = position.x / TILE_SIZE;
    int y = position.y / TILE_SIZE;
    if (ctx->grid.is_out_of_range(x, y)) {
        return false;
    }
    auto& cell = ctx->grid.get(x, y);
    cell.pressed = true;
    _tile = &cell;
    _prev = point;
    return true;
}

void PlayGame::onTouchMoved(mge::Vector2i const& point) {

    if (_tile == nullptr) {
        _gridlayer->getCamera()->move_offset((_prev - point).to<float>());
        _offset = (_prev - point).to<float>();
        _prev = point;
        return;
    }

    if (point.distance(_prev) > 8) {
        _tile->pressed = false;
        _tile = nullptr;
        _prev = point;
        return;
    }
    _prev = point;

    auto world_pos = covertToWorldPosition(point.to<float>());
    auto position = _gridlayer->getLayer(0)->covertToLocalPosition(world_pos);

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

void PlayGame::onTouchEnded(mge::Vector2i const& point) {
    if (_tile) {
        auto& position = _tile->position;
        _tile->pressed = false;
        _tile = nullptr;
        if (ctx->grid.is_out_of_range(position)) {
            return;
        }
        click_tile(*ctx, position);
    }
    if (_offset.distance({0.0f, 0.0f}) > 8) {
        _move = _offset;
        _offset.reset();
    }
}

mge::GridMapWidget* PlayGame::grid() const {
    return _gridlayer;
}

void PlayGame::restart() {
    restart_game(*ctx, {ctx->setting.map_width, ctx->setting.map_height}, ctx->setting.mine_number);
}

void PlayGame::reload_data() {
    _gridlayer->reload_data();
}

size_t PlayGame::numberOfLayersInWidget(mge::GridMapWidget const* sender) {
    return 1;
}

mge::Vector2i PlayGame::sizeOfGridMap(mge::GridMapWidget const* sender) {
    return ctx->grid.size();
}

mge::Vector2i PlayGame::sizeOfGridTile(mge::GridMapWidget const* sender) {
    return {TILE_SIZE, TILE_SIZE};
}

Widget::Ptr PlayGame::tileWidgetAtPosition(mge::GridMapWidget* sender, int layerIndex, mge::Vector2i const& position) {
    auto view = sender->dequeueTile(layerIndex);
    if (view == nullptr) {
        view.reset(new MineTileCell);
    }
    view->fast_to<MineTileCell>()->setData(&ctx->grid.get(position));
    return view;
}