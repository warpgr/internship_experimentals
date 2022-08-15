#pragma once

#include <shared_state_impl/shared_state_interface.hpp>
#include <concepts>

namespace il { namespace impl {

template<typename T>
class shared_state_recv {
    std::shared_ptr<shared_state<T>> state_ = { nullptr };
public:
    shared_state_recv() { state_ = std::make_shared<shared_state<T>>(); }
    shared_state_recv(const std::shared_ptr<shared_state<T>>& state) { state_ = state; }
    shared_state_recv(std::shared_ptr<shared_state<T>>&& state) { state_ = std::move(state); }

    shared_state_recv(const shared_state_recv& other) = delete;
    shared_state_recv(shared_state_recv&& other) = delete;
    shared_state_recv& operator=(const shared_state_recv& other) = delete;
    shared_state_recv&& operator=(shared_state_recv&& other) = delete;
public:
    T get() {
        have_correct_state(state_);
        return state_->get();
    }
    void wait() {
        have_correct_state(state_);
        return state_->wait();
    }
    template <typename Rep, typename Period = std::ratio<1>>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        have_correct_state(state_);
        return state_->wait_for(duration);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
    void wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
        have_correct_state(state_);
        return state_->wait_until(timeout_time);
    }

    template <typename Func>
    auto then(Func&& func, const launch& launch_type = launch::asynchronious) ->
        future<decltype(func(T()))> {
        return state_->then(std::forward<Func>(func), launch_type);
    }

    std::shared_ptr<shared_state<T>> state() {
        return state_;
    }
};

}}