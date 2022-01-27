//
// Created by baifeng on 2021/11/19.
//

#ifndef SDL2_UI_GRIDMAP_H
#define SDL2_UI_GRIDMAP_H

#include "widget.h"
#include "grid.h"
#include "rect.h"
#include "observer.h"

mge_begin

class TileWidget : public Widget {
public:
    TileWidget() {}
    inline void setTileIndex(size_t index) {
        _index = index;
    }
    inline size_t getTileIndex() const {
        return _index;
    }
protected:
    size_t _index;
};

class GridMapWidget;
class GridMapDataSource {
public:
    virtual ~GridMapDataSource() {}
public:
    virtual size_t numberOfLayersInWidget(GridMapWidget const* sender) = 0;
    virtual Vector2i sizeOfGridMap(GridMapWidget const* sender) = 0;
    virtual Vector2i sizeOfGridTile(GridMapWidget const* sender) = 0;
    virtual Widget::Ptr tileWidgetAtPosition(GridMapWidget* sender, int layerIndex, Vector2i const& position) = 0;
};

class GridMapWidget;
class GridMapLayer : public WindowWidget {
    friend class GridMapWidget;
public:
    GridMapLayer(GridMapWidget* gridmap, int layerIndex);
protected:
    Widget::Ptr dequeueTile();
    void enqueueTile(Widget::Ptr const& widget);
    void makeTile(Vector2i const& tile_pos, Vector2i const& map_size, Vector2i const& tile_size);
    void checkTiles();
protected:
    typedef std::list<Widget::Ptr> TileQueue;
    typedef std::unordered_map<int, TileWidget*> TilePool;
    int _layerIndex;
    GridMapWidget* _gridmap;
    TileQueue _idleTiles;
    TileQueue _busyTiles;
    TilePool _tiles;
};

class GridMapCamera : public WindowWidget {
public:
    enum EVENT {
        DID_SCROLL = 0xABEF2001,
    };
    enum MoveDirection {
        MOVE_UP = 0,
        MOVE_RIGHT,
        MOVE_DOWN,
        MOVE_LEFT,
    };
    struct Padding {
        uint32_t top;
        uint32_t right;
        uint32_t bottom;
        uint32_t left;
    };
    typedef std::vector<MoveDirection> MoveDirs;
    GridMapCamera(Widget* container);
public:
    void follow(Vector2f const& position);
    void follow(Vector2f const& position, bool limit_camera);
    void move(Vector2f const& speed);
    void move_offset(Vector2f const& offset);
    MoveDirs const& move_dirs() const;
    bool inCamera(RectI const& r) const;
    void setCameraPosition(Vector2f const& position);
    Vector2f getCameraPosition() const;
    Vector2f getFollowPosition() const;
    void limitCamera();
    void setPadding(Padding const& padding);
protected:
    void onUpdate(float delta) override;
    void update_dirs(Vector2f const& move);
    void limit_with_dirs();
    void limitTop();
    void limitRight();
    void limitBottom();
    void limitLeft();
protected:
    bool _move;
    Widget* _container;
    Vector2f _speed;
    MoveDirs _moveDirs;
    Padding _padding;
};

class GridMapWidget : public WindowWidget {
public:
    enum EVENT {
        ON_DATA_RELOAD = 0xABEF3001,
    };
public:
    typedef GridMapDataSource DataSource;
public:
    GridMapWidget();
public:
    void setDataSource(DataSource* data_source);
    DataSource* getDataSource() const;
    GridMapLayer* getLayer(int index) const;
    GridMapCamera* getCamera() const;
    Vector2i getContainerSize() const;
public:
    void reload_data();
    Widget::Ptr dequeueTile(int layer);
protected:
    void checkTiles(Widget* sender);
    void onModifySize(Vector2f const& size) override;
protected:
    typedef std::vector<GridMapLayer*> GridLayers;
    DataSource* _dataSource;
    WindowWidget* _container;
    GridMapCamera* _camera;
    GridLayers _tileLayers;
};

mge_end

#endif //SDL2_UI_GRIDMAP_H
