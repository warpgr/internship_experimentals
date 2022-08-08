#pragma once
#include <concurrent/condition_variable.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <exception>
#include <chrono>

namespace il { namespace impl {

template <typename T>
class shared_state {
protected:
    il::condition_variable<il::unique_lock<il::mutex>> is_ready_;
    il::mutex                                          guard_;
    T                                                  shared_data_;
    std::exception*                                    ex_ptr_ = nullptr;
public:
    shared_state() = default;
    shared_state(shared_state&& other) = delete;
    shared_state(shared_state& other) = delete;
    shared_state&& operator=(shared_state&& other) = delete;
    shared_state& operator=(shared_state& other) = delete;
    virtual ~shared_state() = 0;
};
}}

