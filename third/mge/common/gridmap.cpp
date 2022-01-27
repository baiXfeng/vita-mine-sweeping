//
// Created by baifeng on 2021/11/19.
//

#include "gridmap.h"
#include "collision.h"
#include <assert.h>

mge_begin

    //=====================================================================================

    GridMapLayer::GridMapLayer(GridMapWidget* gridmap, int layerIndex):_layerIndex(layerIndex), _gridmap(gridmap) {

    }

    Widget::Ptr GridMapLayer::dequeueTile() {
        if (_idleTiles.empty()) {
            return nullptr;
        }
        auto r = _idleTiles.back();
        _idleTiles.pop_back();
        return r;
    }

    void GridMapLayer::enqueueTile(Widget::Ptr const& widget) {
        _busyTiles.push_back(widget);
    }

    void GridMapLayer::makeTile(Vector2i const& tile_pos, Vector2i const& map_size, Vector2i const& tile_size) {
        auto cell = _gridmap->getDataSource()->tileWidgetAtPosition(_gridmap, _layerIndex, tile_pos);
        if (cell == nullptr) {
            return;
        }

        int x = tile_pos.x;
        int y = tile_pos.y;

        cell->setVisible(true);
        cell->setPosition(x * tile_size.x, y * tile_size.y);
        cell->setSize(tile_size.to<float>());

        auto tile_view = cell->to<TileWidget>();
        auto tile_index = y * map_size.x + x;
        tile_view->setTileIndex(tile_index);
        _tiles[tile_index] = tile_view;

        if (cell->parent() == nullptr) {
            this->addChild(cell);
        }
        this->enqueueTile(cell);
    }

    void GridMapLayer::checkTiles() {

        auto const& position = _gridmap->getCamera()->getCameraPosition();
        auto const& size = _gridmap->getCamera()->size();
        auto map_size = _gridmap->getDataSource()->sizeOfGridMap(_gridmap);
        auto tile_size = _gridmap->getDataSource()->sizeOfGridTile(_gridmap);

        int begin_x = position.x / tile_size.x;
        int begin_y = position.y / tile_size.y;
        int end_x = ceil(size.x / float(tile_size.x)) + begin_x + 1;
        int end_y = ceil(size.y / float(tile_size.y)) + begin_y + 1;

        begin_x = begin_x <= -1 ? 0 : begin_x;
        begin_y = begin_y <= -1 ? 0 : begin_y;
        end_x = end_x >= map_size.x ? map_size.x : end_x;
        end_y = end_y >= map_size.y ? map_size.y : end_y;

        // 检查是否需要刷新地图
        if (auto skip_refresh = false; !skip_refresh) {
            Vector2i points[4] = {
                    {begin_x,   begin_y},
                    {end_x - 1, begin_y},
                    {end_x - 1, end_y - 1},
                    {begin_x, end_y - 1},
            };
            skip_refresh = true;
            for (int i = 0; i < 4; ++i) {
                auto const& pos = points[i];
                auto id = pos.y * map_size.x + pos.x;
                if (_tiles.find(id) == _tiles.end()) {
                    skip_refresh = false;
                    break;
                }
            }
            if (skip_refresh) {
                return;
            }
        }

        // 记录需要移除的砖块
        std::vector<int> idle_indexs;
        std::vector<TileWidget*> idle_tiles;
        for (auto& kv : _tiles) {
            int x = kv.first % map_size.x;
            int y = kv.first / map_size.x;
            if (x < begin_x or y < begin_y or x >= end_x or y >= end_y) {
                idle_indexs.emplace_back(kv.first);
                idle_tiles.emplace_back(kv.second);
            }
        }
        // 移除砖块视图
        for (auto& tileView : idle_tiles) {
            tileView->setVisible(false);
            for (auto iter = _busyTiles.begin(); iter != _busyTiles.end(); iter++) {
                if (iter->get() == tileView) {
                    _idleTiles.emplace_back(*iter);
                    _busyTiles.erase(iter);
                    break;
                }
            }
        }
        for (auto& id : idle_indexs) {
            _tiles.erase(id);
        }
        // 补齐砖块
        for (int y = begin_y; y < end_y; ++y) {
            for (int x = begin_x; x < end_x; ++x) {
                int id = y * map_size.x + x;
                if (_tiles.find(id) == _tiles.end()) {
                    makeTile({x, y}, map_size, tile_size);
                }
            }
        }
        //printf("x = %d, y = %d, x2 = %d, y2 = %d\n", begin_x, begin_y, end_x, end_y);
        //printf("idle = %d, busy = %d, child = %d, pool = %d\n", _idleTiles.size(), _busyTiles.size(), _children.size(), _tiles.size());
    }

    //=====================================================================================

    GridMapCamera::GridMapCamera(Widget* container):_container(container), _move(false), _padding({0, 0, 0, 0}) {}

    void GridMapCamera::onUpdate(float delta) {
        if (_move) {
            move_offset(_speed * delta);
        }
    }

    void GridMapCamera::update_dirs(Vector2f const& move) {
        _moveDirs.clear();
        int x = int(move.x);
        if (x >= 1) {
            _moveDirs.push_back(MOVE_LEFT);
        } else if (x <= -1) {
            _moveDirs.push_back(MOVE_RIGHT);
        }
        int y = int(move.y);
        if (y >= 1) {
            _moveDirs.push_back(MOVE_UP);
        } else if (y <= -1) {
            _moveDirs.push_back(MOVE_DOWN);
        }
    }

    void GridMapCamera::limit_with_dirs() {
        for (auto& dir : _moveDirs) {
            if (dir == MOVE_LEFT) {
                limitLeft();
            } else if (dir == MOVE_RIGHT) {
                limitRight();
            } else if (dir == MOVE_UP) {
                limitTop();
            } else if (dir == MOVE_DOWN) {
                limitBottom();
            }
        }
    }

    void GridMapCamera::limitTop() {
        if (int(_container->size().y) > int(size().y)) {
            if (_container->position().y >= _padding.top) {
                _container->setPositionY(_padding.top);
            }
        } else {
            int limit_y = size().y - _container->size().y;
            if (limit_y <= _padding.top) {
                limit_y = _padding.top;
            }
            if (_container->position().y >= limit_y) {
                _container->setPositionY(limit_y);
            }
        }
    }

    void GridMapCamera::limitRight() {
        if (int(_container->size().x) > int(size().x)) {
            int limit_x = size().x - _container->size().x - _padding.right;
            if (_container->position().x <= limit_x) {
                _container->setPositionX(limit_x);
            }
        } else {
            if (int limit_x = size().x - _container->size().x - _padding.right; limit_x <= 0 and _padding.right != 0) {
                if (_container->position().x <= limit_x) {
                    _container->setPositionX(limit_x);
                }
            } else if (_container->position().x <= 0.0f) {
                _container->setPositionX(0.0f);
            }
        }
    }

    void GridMapCamera::limitBottom() {
        if (int(_container->size().y) > int(size().y)) {
            int limit_y = size().y - _container->size().y - _padding.bottom;
            if (_container->position().y <= limit_y) {
                _container->setPositionY(limit_y);
            }
        } else {
            if (int limit_y = size().y - _container->size().y - _padding.bottom; limit_y <= 0 and _padding.bottom != 0) {
                if (_container->position().y <= limit_y) {
                    _container->setPositionY(limit_y);
                }
            } else if (_container->position().y <= 0.0f) {
                _container->setPositionY(0.0f);
            }
        }
    }

    void GridMapCamera::limitLeft() {
        if (int(_container->size().x) > int(size().x)) {
            if (_container->position().x >= _padding.left) {
                _container->setPositionX(_padding.left);
            }
        } else {
            int limit_x = size().x - _container->size().x;
            if (limit_x <= _padding.left) {
                limit_x = _padding.left;
            }
            if (_container->position().x >= limit_x) {
                _container->setPositionX(limit_x);
            }
        }
    }

    void GridMapCamera::follow(Vector2f const& position) {
        follow(position, true);
    }

    void GridMapCamera::follow(Vector2f const& position, bool limit_camera) {
        if (_move) {
            return;
        }
        auto old_position = _container->position();
        this->setCameraPosition(position - size() * 0.5f);
        if (limit_camera) {
            this->limitCamera();
        }
        this->update_dirs(_container->position() - old_position);
        signal(DID_SCROLL)(this);
    }

    void GridMapCamera::move_offset(Vector2f const& offset) {
        auto old_position = _container->position();
        _container->setPosition(_container->position() + offset * -1);
        this->limitCamera();
        this->update_dirs(_container->position() - old_position);
        signal(DID_SCROLL)(this);
    }

    void GridMapCamera::move(Vector2f const& speed) {
        _speed = speed;
        _move = int(_speed.x * 1000) != 0 or int(_speed.y * 1000) != 0;
    }

    GridMapCamera::MoveDirs const& GridMapCamera::move_dirs() const {
        return _moveDirs;
    }

    bool GridMapCamera::inCamera(RectI const& r) const {
        return isCollision(RectI{
            int(_container->position().x + r.x),
            int(_container->position().y + r.y),
            int(r.w),
            int(r.h),
        }, RectI{
            int(position().x),
            int(position().y),
            int(size().x),
            int(size().y),
        });
    }

    void GridMapCamera::setCameraPosition(Vector2f const& position) {
        _container->setPosition(position * -1);
    }

    Vector2f GridMapCamera::getCameraPosition() const {
        return _container->position() * -1;
    }

    Vector2f GridMapCamera::getFollowPosition() const {
        return getCameraPosition() + size() * 0.5f;
    }

    void GridMapCamera::limitCamera() {
        limitTop();
        limitRight();
        limitBottom();
        limitLeft();
    }

    void GridMapCamera::setPadding(Padding const& padding) {
        _padding = padding;
    }

    //=====================================================================================

    GridMapWidget::GridMapWidget():_dataSource(nullptr), _container(new WindowWidget) {
        addChild(Ptr(_camera = new GridMapCamera(_container)));
        addChild(Ptr(_container));
        _camera->connect(_camera->DID_SCROLL, std::bind(&GridMapWidget::checkTiles, this, std::placeholders::_1));
    }

    void GridMapWidget::setDataSource(DataSource* data_source) {
        _dataSource = data_source;
    }

    GridMapWidget::DataSource* GridMapWidget::getDataSource() const {
        return _dataSource;
    }

    GridMapLayer* GridMapWidget::getLayer(int index) const {
        return _tileLayers[index];
    }

    Vector2i GridMapWidget::getContainerSize() const {
        auto map_size = _dataSource->sizeOfGridMap(this);
        auto tile_size = _dataSource->sizeOfGridTile(this);
        return map_size * tile_size;
    }

    GridMapCamera* GridMapWidget::getCamera() const {
        return _camera;
    }

    void GridMapWidget::reload_data() {
        assert(_dataSource != nullptr && "GridMapWidget::reload_data fail.");

        _tileLayers.clear();
        _container->removeAllChildren();
        _camera->setSize(size());

        auto map_size = _dataSource->sizeOfGridMap(this);
        auto tile_size = _dataSource->sizeOfGridTile(this);
        auto view_size = map_size * tile_size;
        _container->setSize(view_size.to<float>());
        _camera->limitCamera();

        auto layer_number = _dataSource->numberOfLayersInWidget(this);
        for (int i = 0; i < layer_number; ++i) {
            auto layer = New<GridMapLayer>(this, i);
            layer->setSize(_container->size());
            _container->addChild(layer);
            _tileLayers.push_back(layer->fast_to<GridMapLayer>());
        }

        auto begin_position = _container->position() * -1;
        int begin_x = begin_position.x / tile_size.x;
        int begin_y = begin_position.y / tile_size.y;
        for (int y = begin_y; y < map_size.y; ++y) {
            if (y <= -1) {
                continue;
            }
            for (int x = begin_x; x < map_size.x; ++x) {
                if (x <= -1) {
                    continue;
                }
                for (int i = 0; i < layer_number; ++i) {
                    _tileLayers[i]->makeTile({x, y}, map_size, tile_size);
                }
                if (_container->position().x + (x + 1) * tile_size.x >= size().x) {
                    break;
                }
            }
            if (_container->position().y + (y + 1) * tile_size.y >= size().y) {
                break;
            }
        }

        signal(ON_DATA_RELOAD)(this);
    }

    void GridMapWidget::onModifySize(Vector2f const& size) {
        _camera->setSize(size);
    }

    void GridMapWidget::checkTiles(Widget* sender) {
        if (_camera->move_dirs().empty()) {
            return;
        }
        for (auto& layer : _tileLayers) {
            layer->checkTiles();
        }
    }

    Widget::Ptr GridMapWidget::dequeueTile(int layer) {
        assert(layer >= 0 && "GridMapWidget::dequeueTile error.");
        if (layer >= _tileLayers.size()) {
            return nullptr;
        }
        return _tileLayers[layer]->dequeueTile();
    }

mge_end