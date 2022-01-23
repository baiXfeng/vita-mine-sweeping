//
// Created by baifeng on 2022/1/23.
//

#include "logic.h"
#include "context.h"
#include "tile.h"
#include <list>

bool restart_game(Context& c, mge::Vector2i const& mapSize, uint32_t number_of_mine) {

    assert(number_of_mine < mapSize.x * mapSize.y && "restart_game fail.");

    c.flag = false;
    c.grid.resize(mapSize, {});
    for (int i = 0; i < c.grid.cells().size(); ++i) {
        auto& cell = c.grid.cells()[i];
        cell.id = i + 1;
        cell.flag = &c.flag;
    }

    // 生成地雷
    {
        auto const tile_amount = c.grid.cells().size();
        auto& tiles = c.grid.cells();

        for (int i = 0; i < number_of_mine; ++i) {
            c.grid.cells()[i].is_mine = true;
        }

        // 随机打乱地雷位置
        for (int i = 0; i < tile_amount; ++i) {
            auto index = rand() % tile_amount;
            if (i == index) {
                continue;
            }
            auto temp = tiles[i];
            tiles[i] = tiles[index];
            tiles[index] = temp;
        }
    }

    // 生成数字
    auto get_number = [&c](mge::Vector2i const& position) {
        mge::Vector2i offset[8] = {
                {-1, -1},
                {0, -1},
                {1, -1},
                {-1, 0},
                {1, 0},
                {-1, 1},
                {0, 1},
                {1, 1},
        };
        int ret = 0;
        for (int i = 0; i < 8; ++i) {
            auto pos = position + offset[i];
            if (c.grid.is_out_of_range(pos)) {
                continue;
            }
            auto& cell = c.grid.get(pos);
            if (cell.is_mine) {
                ret++;
            }
        }
        return ret;
    };
    for (int y = 0; y < c.grid.size().y; ++y) {
        for (int x = 0; x < c.grid.size().x; ++x) {
            auto& cell = c.grid.get(x, y);
            if (!cell.is_mine) {
                cell.number = get_number({x, y});
            }
        }
    }

    return true;
}

bool click_tile(Context& c, mge::Vector2i const& position) {

    if (c.grid.is_out_of_range(position)) {
        return true;
    }

    auto& cell = c.grid.get(position);

    if (cell.hidden) {
        return open_tile(c, position);
    }

    if (cell.flag and *cell.flag) {
        return auto_clear(c, position);
    }

    return false;
}

bool open_tile(Context& c, mge::Vector2i const& position) {

    auto& cell = c.grid.get(position);

    if (cell.number >= 1) {
        cell.hidden = false;
        return true;
    }

    if (cell.is_mine) {
        // 点中地雷，游戏结束
        printf("点中地雷.\n");
        return true;
    }

    return clear_empty(c, position);
}

bool auto_clear(Context& c, mge::Vector2i const& position) {
    return false;
}

class AStarClearEmpty {
    class Node {
    public:
        Node():id(0), prev(nullptr) {}
        Node(int id, mge::Vector2i const& pos, Node* prev = nullptr) {
            this->id = id;
            this->pos = pos;
            this->prev = prev;
        }
        int id;
        mge::Vector2i pos;
        Node* prev;
    };
    typedef std::shared_ptr<Node> Nodep;
    typedef std::list<Nodep> NodeList;
public:
    AStarClearEmpty(Context& c):ctx(c) {}
    void clear(mge::Vector2i const& position) {

        auto& cell = ctx.grid.get(position);
        _open.emplace_back(Nodep(new Node(cell.id, position)));
        Node* current = nullptr;

        mge::Vector2i offset[4] = {
                {0, -1},
                {1, 0},
                {0, 1},
                {-1, 0},
        };
        mge::Vector2i bevel_offset[4] = {
                {-1, -1},
                {1, -1},
                {-1, 1},
                {1, 1},
        };

        while (!_open.empty()) {

            auto current_iter = _open.begin();
            current = current_iter->get();
            _closed.emplace_back(*current_iter);
            _open.erase(current_iter);

            // 翻开当前空白块
            ctx.grid.get(current->pos).hidden = false;

            // 把斜角的数字翻开
            for (int i = 0; i < 4; ++i) {
                auto next_pos = current->pos + bevel_offset[i];
                if (ctx.grid.is_out_of_range(next_pos)) {
                    continue;
                }
                if (auto& cell = ctx.grid.get(next_pos); cell.number >= 1) {
                    cell.hidden = false; // 数字直接翻开
                }
            }

            // 把上下左右四个方向的空白块添加进列表等待翻开
            for (int i = 0; i < 4; ++i) {

                auto next_pos = current->pos + offset[i];
                if (ctx.grid.is_out_of_range(next_pos)) {
                    continue;
                }

                auto& cell = ctx.grid.get(next_pos);

                if (cell.number >= 1) {
                    cell.hidden = false; // 数字直接翻开
                    continue;
                }

                if (auto node = findNode(_closed, cell.id); node) {
                    // 已经翻开，跳过
                    continue;
                }

                if (auto node = findNode(_open, cell.id); !node) {
                    _open.emplace_back(Nodep(new Node(cell.id, next_pos)));
                }
            }
        }
    }
    Node* findNode(NodeList const& list, int id) {
        for (auto& n : list) {
            if (n->id == id) {
                return n.get();
            }
        }
        return nullptr;
    }
private:
    Context& ctx;
    NodeList _open;
    NodeList _closed;
};

bool clear_empty(Context& c, mge::Vector2i const& position) {
    AStarClearEmpty ce(c);
    ce.clear(position);
    return true;
}
