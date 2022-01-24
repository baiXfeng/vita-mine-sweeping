//
// Created by baifeng on 2022/1/23.
//

#include "logic.h"
#include "context.h"
#include "common/game.h"
#include "common/event.h"
#include <list>

bool restart_game(Context& c, mge::Vector2i const& mapSize, uint32_t number_of_mine) {

    assert(number_of_mine < mapSize.x * mapSize.y && "restart_game fail.");

    srand(time(nullptr));

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

    // 游戏开始通知
    _game.event().notify(mge::Event(EVENT_ID::GAME_START));

    // 更新游戏时间显示
    c.seconds = 0.0f;
    c.notify(&ContextObserver::onTimeModify, c.seconds);

    // 更新地雷数量显示
    c.max_mine_number = number_of_mine;
    c.mine_number = number_of_mine;
    c.notify(&ContextObserver::onMineNumberModify, c.mine_number);

    c.flag = false;
    c.finished = false;
    c.first_click = true;
    return true;
}

bool click_tile(Context& c, mge::Vector2i const& position) {

    if (c.grid.is_out_of_range(position)) {
        return true;
    }

    auto& cell = c.grid.get(position);

    if (cell.flag_on) {
        if (c.flag) {
            return set_flag(c, position);
        }
        return true;
    }

    if (cell.hidden) {
        if (c.flag and !c.first_click) {
            return set_flag(c, position);
        }
        c.first_click = false;
        return open_tile(c, position);
    }

    if (cell.number >= 1) {
        return auto_clear(c, position);
    }

    return false;
}

class CheckWin {
public:
    CheckWin(Context& c):ctx(c) {}
    ~CheckWin() {
        check();
    }
private:
    void check() {
        if (ctx.finished) {
            return;
        }
        int hidden_number = 0;
        std::vector<Tile*> mine_array;
        for (auto& cell : ctx.grid.cells()) {
            if (cell.hidden) {
                hidden_number++;
            }
            if (cell.is_mine) {
                mine_array.emplace_back(&cell);
            }
        }
        if (hidden_number == ctx.max_mine_number) {
            for (auto cell : mine_array) {
                cell->flag_on = true;
            }
            ctx.mine_number = 0;
            ctx.notify(&ContextObserver::onMineNumberModify, ctx.mine_number);
            ctx.finished = true;
            _game.event().notify(EVENT_ID::GAME_WIN);
        }
    }
    Context& ctx;
};

bool open_tile(Context& c, mge::Vector2i const& position) {

    auto& cell = c.grid.get(position);

    if (cell.flag_on) {
        return true;
    }

    if (cell.number >= 1) {
        CheckWin check(c);
        cell.hidden = false;
        return true;
    }

    if (cell.is_mine) {
        // 点中地雷，游戏结束
        cell.bomb = true;
        cell.hidden = false;
        clear_all_mine(c);
        c.finished = true;
        _game.event().notify(mge::Event(EVENT_ID::GAME_OVER));
        return true;
    }

    return clear_empty(c, position);
}

bool auto_clear(Context& c, mge::Vector2i const& position) {

    mge::Vector2i offset[8] = {
            {0, -1},
            {1, 0},
            {0, 1},
            {-1, 0},
            {-1, -1},
            {1, -1},
            {-1, 1},
            {1, 1},
    };

    int mine_number = 0;
    for (int i = 0; i < 8; ++i) {
        auto pos = position + offset[i];
        if (c.grid.is_out_of_range(pos)) {
            continue;
        }
        auto& cell = c.grid.get(pos);
        mine_number += cell.flag_on ? 1 : 0;
    }

    // 旗子数量与数字提示数量不相符，不能自动消除
    auto& cell = c.grid.get(position);
    if (mine_number != cell.number) {
        return false;
    }

    // 翻开周边块
    for (int i = 0; i < 8; ++i) {
        auto pos = position + offset[i];
        if (c.grid.is_out_of_range(pos)) {
            continue;
        }
        open_tile(c, pos);
    }

    return true;
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
                    continue;
                } else if (cell.hidden) {
                    // 翻开隐藏的空白快
                    if (!cell.is_mine and cell.number == 0) {
                        open_tile(ctx, next_pos);
                    }
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
    CheckWin check(c);
    AStarClearEmpty ce(c);
    ce.clear(position);
    return true;
}

bool set_flag(Context& c, mge::Vector2i const& position) {

    auto& cell = c.grid.get(position);
    cell.flag_on = !cell.flag_on;

    if (cell.flag_on) {
        c.mine_number--;
    } else {
        c.mine_number++;
    }
    c.notify(&ContextObserver::onMineNumberModify, c.mine_number);

    return true;
}

bool clear_all_mine(Context& c) {
    for (auto& cell : c.grid.cells()) {
        if (!cell.hidden) {
            continue;
        }
        if (!cell.is_mine and cell.flag_on) {
            // 标记错了地雷
            cell.flag_wrong = true;
            continue;
        }
        if (cell.is_mine and !cell.flag_on) {
            cell.hidden = false;
            continue;
        }
    }
    return true;
}
