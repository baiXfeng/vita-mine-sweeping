//
// Created by baifeng on 2021/11/8.
//

#ifndef SDL2_UI_OBSERVER_H
#define SDL2_UI_OBSERVER_H

#include <list>
#include <algorithm>
#include <functional>
#include "macro.h"

mge_begin

template<class T>
class Observer {
public:
    typedef T View;
public:
    virtual ~Observer() {}
public:
    void add(View* view) {
        _views.push_back(view);
    }
    void remove(View* view) {
        auto iter = std::find(_views.begin(), _views.end(), view);
        if (iter != _views.end()) {
            _views.erase(iter);
        }
    }
    template<typename... Args>
    void notify(void (View::*method)(Args...), Args const&... args) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(args...);
        }
    }
    template<typename T1>
    void notify(void (View::*method)(T1 const&), T1 const& arg1) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(arg1);
        }
    }
    template<typename T1, typename T2>
    void notify(void (View::*method)(T1 const&, T2 const&), T1 const& arg1, T2 const& arg2) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(arg1, arg2);
        }
    }
    template<typename T1, typename T2, typename T3>
    void notify(void (View::*method)(T1 const&, T2 const&, T3 const&), T1 const& arg1, T2 const& arg2, T3 const& arg3) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(arg1, arg2, arg3);
        }
    }
    template<typename T1, typename T2, typename T3, typename T4>
    void notify(void (View::*method)(T1 const&, T2 const&, T3 const&, T4 const&), T1 const& arg1, T2 const& arg2, T3 const& arg3, T4 const& arg4) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(arg1, arg2, arg3, arg4);
        }
    }
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void notify(void (View::*method)(T1 const&, T2 const&, T3 const&, T4 const&, T5 const&), T1 const& arg1, T2 const& arg2, T3 const& arg3, T4 const& arg4, T5 const& arg5) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)(arg1, arg2, arg3, arg4, arg5);
        }
    }
    void notify(void (View::*method)()) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            (v->*method)();
        }
    }
    void each(std::function<void(View* v)> const& catcher) {
        if (_views.empty()) {
            return;
        }
        for (auto& v : _views) {
            catcher(v);
        }
    }
protected:
    std::list<View*> _views;
};

namespace __private__ {
    static int signal_func_id = 0;
}

template<typename T>
class Signal {
public:
    typedef std::function<T> observer_type;
private:
    class Slot {
        friend class Signal<T>;
    public:
        Slot():_id(0) {}
        Slot(observer_type const& obs):_obs(obs), _id(++__private__::signal_func_id) {}
        Slot(Slot const& slot) {
            *this = slot;
        }
        Slot& operator=(Slot const& slot) {
            _id = slot._id;
            _obs = slot._obs;
            return *this;
        }
        bool operator==(Slot const& slot) const {
            return _id == slot._id;
        }
    private:
        observer_type& get() {
            return _obs;
        }
    private:
        int _id;
        observer_type _obs;
    };
public:
    typedef Slot slot_type;
    typedef std::list<slot_type> oberser_list;
public:
    virtual ~Signal() {}
public:
    template<typename... Args>
    void operator()(Args const&... args) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(args...);
        }
    }
    template<typename T1>
    void notify(T1 const& arg1) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(arg1);
        }
    }
    template<typename T1, typename T2>
    void notify(T1 const& arg1, T2 const& arg2) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(arg1, arg2);
        }
    }
    template<typename T1, typename T2, typename T3>
    void notify(T1 const& arg1, T2 const& arg2, T3 const& arg3) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(arg1, arg2, arg3);
        }
    }
    template<typename T1, typename T2, typename T3, typename T4>
    void notify(T1 const& arg1, T2 const& arg2, T3 const& arg3, T4 const& arg4) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(arg1, arg2, arg3, arg4);
        }
    }
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void notify(T1 const& arg1, T2 const& arg2, T3 const& arg3, T4 const& arg4, T5 const& arg5) {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()(arg1, arg2, arg3, arg4, arg5);
        }
    }
    void operator()() {
        if (_list.empty()) {
            return;
        }
        for (auto& obs : _list) {
            obs.get()();
        }
    }
    slot_type connect(observer_type const& obs) {
        _list.push_back(obs);
        return _list.back();
    }
    void disconnect(Slot const& target) {
        auto iter = std::find(_list.begin(), _list.end(), target);
        if (iter != _list.end()) {
            _list.erase(iter);
        }
    }
    void clear() {
        _list.clear();
    }
    bool empty() const {
        return _list.empty();
    }
    int size() const {
        return _list.size();
    }
protected:
    oberser_list _list;
};

mge_end

#endif //SDL2_UI_OBSERVER_H
