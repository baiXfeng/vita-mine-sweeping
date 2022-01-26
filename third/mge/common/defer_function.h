//
// Created by baifeng on 2022/1/26.
//

#ifndef SDL2_UI_FUNCTION_DEFER_H
#define SDL2_UI_FUNCTION_DEFER_H

#include <functional>
#include "macro.h"

mge_begin

class DeferCallBack {
public:
    virtual ~DeferCallBack() {}
    virtual void call() {}
};

template<typename T>
class DeferWithData : public DeferCallBack {
    typedef std::function<void(T*)> Function;
    typedef T Data;
public:
    DeferWithData(Function const& func, Data* data):_f(func), _d(data) {}
private:
    void call() override {
        _f(_d);
    }
    Function _f;
    Data* _d;
};

class DeferVoid : public DeferCallBack {
    typedef std::function<void()> Function;
public:
    DeferVoid(Function const& func):_f(func) {}
private:
    void call() override {
        _f();
    }
    Function _f;
};

template<typename T>
class Defer final {
    typedef T DATA;
public:
    Defer(std::function<void(DATA*)> const& f, DATA* d):_defer(new DeferWithData<DATA>(f, d)) {}
    Defer(std::function<void()> const& f):_defer(new DeferVoid(f)) {}
    ~Defer() {
        if (_defer) {
            _defer->call();
            delete _defer;
        }
    }
private:
    DeferCallBack* _defer;
};

mge_end

#endif //SDL2_UI_FUNCTION_DEFER_H
