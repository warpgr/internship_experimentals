#pragma once
#include <concurrent/condition_variable.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <exception>
#include <chrono>
#include <memory>
#include <atomic>
#include <iostream>
#include <functional>
#include <concurrent/thread_pool.hpp>

namespace il { 
template <typename T>
class future;
template <typename T>
class promise;
enum launch {
    asynchronious = 1,
    deferred = 2
};
namespace impl {

#define have_correct_state(state) assert(nullptr != state);
#define is_state_shared(shared) assert(shared.load());
#define is_state_shared_and_is_not_setted(shared, setted ) \
    is_state_shared(shared)\
    assert(!setted.load());


template <typename T>
class shared_state {
    unique_lock<mutex> wait_() {
        unique_lock<mutex> lock(guard_);
        is_ready_.wait(lock, [&] { return is_setted_.load(); });
        return lock;
    }
    unique_lock<mutex> wait_for_set() {
        unique_lock<mutex> lock(guard_);
        if (!is_first_.exchange(false)) {
            is_ready_.wait(lock, [&] { return !is_setted_.load(); });
        }
        return lock;
    }
protected:
    condition_variable<unique_lock<mutex>>              is_ready_;
    mutex                                               guard_;
    T                                                   shared_data_; // TODO: must be with allocator // shared_ptr
    std::shared_ptr<std::exception>                     ex_ptr_ = nullptr;
    std::atomic<bool>                                   is_first_  = { true };
    std::atomic<bool>                                   is_setted_ = { false };
    std::function<void()>                               callback_;
    std::function<void()>                               on_complete_;
public:
    shared_state() { };
    shared_state(shared_state&& other) = delete;
    shared_state(shared_state& other) = delete;
    shared_state&& operator=(shared_state&& other) = delete;
    shared_state& operator=(shared_state& other) = delete;

public: // Receiver interface
    T get() {
        if (callback_) { callback_(); }
        if (on_complete_) { on_complete_(); }
        auto lock = wait_();
        if (nullptr != ex_ptr_) {
            throw ex_ptr_;
        }
        T tmp(std::move(shared_data_));
        is_setted_.store(false);
        is_ready_.notify_one();
        return tmp;
    }
    void wait() {
       wait_();
    }
    template <typename Rep, typename Period = std::ratio<1>>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        unique_lock<mutex> lock(guard_);
        is_ready_.wait_for(lock, duration);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
    void wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
        unique_lock<mutex> lock(guard_);
        is_ready_.wait_until(lock, timeout_time);
    }
    template <typename Func>
    auto then(Func&& func, const launch& launch_type = launch::asynchronious) ->
        future<decltype(func(T()))> {
        auto prom = std::make_shared<il::promise<decltype(func(T()))>>();
        std::function<void()> on_complete =
            [&, promise_type = prom] () {
                T value = get();
                promise_type->set_value(func(std::forward<T>(value)));
            };
        switch (launch_type) {
            case launch::deferred: {
                on_complete_ = on_complete;
                break;
            }
            case launch::asynchronious:
            default: {
                default_tp<std::function<void()>>().put_task(std::move(on_complete));
                break;
            }
        }
        return prom->get_future();
    }
public: // Sender interface
    void set_value(T& value) {
        auto lock = wait_for_set();
        shared_data_ = value;
        is_setted_.store(true);
        is_ready_.notify_one();
    }
    void set_value(T&& value) {
        auto lock = wait_for_set();
        shared_data_ = std::move(value);
        is_setted_.store(true);
        is_ready_.notify_one();
    }
    void set_value(const T& value) {
        auto lock = wait_for_set();
        shared_data_(value);
        is_setted_.store(true);
        is_ready_.notify_one();
    }
    void set_value() {
        auto lock = wait_for_set();
        shared_data_ = T();
        is_setted_.store(true);
        is_ready_.notify_one();
    }
    void set_exception(const std::exception& ex) {
       set_exception(std::exception(ex));
    }
    void set_exception(std::exception&& ex) {
        auto lock = wait_();
        ex_ptr_ = std::make_shared<std::exception>(std::move(ex));
        is_setted_.store(true);
        is_ready_.notify_one();
    }

    void set_callback(std::function<void()> callback) {
        callback_ = std::move(callback);
    }
};
}}

