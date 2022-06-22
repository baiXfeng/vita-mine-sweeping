//
// Created by baifeng on 2022/2/9.
//

#ifndef SDL2_UI_HANDLER_H
#define SDL2_UI_HANDLER_H

#include "macro.h"
#include <memory>
#include <vector>

mge_begin

class Handler {
public:
    typedef std::shared_ptr<Handler> Ptr;
    typedef std::vector<Ptr> Array;
public:
    virtual ~Handler() {}
};

template<typename T>
class Behavior : public Handler {
public:
    typedef std::shared_ptr<Behavior<T>> Ptr;
    typedef std::vector<Ptr> Array;
public:
    virtual void step_value(T context) {
        step(context);
    }
    virtual void step_pointer(T* context) {
        step(*context);
    }
    virtual void step(T& context) = 0;
};

template<typename T>
class FSM {
public:
    class State : public Behavior<T> {
        friend class FSM;
    public:
        typedef FSM<T>* FSM_P;
        typedef std::shared_ptr<State> Ptr;
    protected:
        FSM<T>& fsm() {
            return *_fsm;
        }
    private:
        FSM_P _fsm;
    };
    typedef std::shared_ptr<FSM<T>> Ptr;
    typedef std::vector<typename State::Ptr> States;
public:
    FSM():_index(0) {}
    FSM(States const& states):_index(0) {
        reset(states);
    }
public:
    void restart() {
        _index = 0;
    }
    void prev_state() {
        _index--;
    }
    void next_state() {
        _index++;
    }
    void seek_state(uint16_t index) {
        _index = index;
    }
    void next_state(short offset) {
        _index += offset;
    }
    void reset(States const& states) {
        _states = states;
        for (auto& state : states) {
            state->_fsm = this;
        }
    }
    void push_state(typename State::Ptr const& state) {
        _states.template emplace_back(state);
        _states.back()->_fsm = this;
    }
    void pop_state() {
        _states.pop_back();
    }
    void step(T& context) {
        if (!end()) {
            _states[_index]->step(context);
        }
    }
    bool end() const {
        return _index >= _states.size();
    }
protected:
    uint16_t _index;
    States _states;
};

template<typename T>
class FSM_Stack : public std::vector<typename FSM<T>::Ptr> {
public:
    typedef FSM<T> FSM_Type;
};

mge_end

#endif //SDL2_UI_HANDLER_H
