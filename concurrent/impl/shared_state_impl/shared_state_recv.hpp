#pragma once

#include <shared_state_impl/shared_state_interface.hpp>
#include <concepts>

// #include <shared_state_impl/shared_state_send.hpp>

namespace il { namespace impl {


template<typename T, typename ConditionVariableType, Mutex mutex_type>
class shared_state_recv {
    std::shared_ptr<shared_state<T,ConditionVariableType, mutex_type>> state_ = { nullptr };
public:
    shared_state_recv() { state_ = std::make_shared<shared_state<T, ConditionVariableType, mutex_type>>(); }
    shared_state_recv(const std::shared_ptr<shared_state<T, ConditionVariableType, mutex_type>>& state) { state_ = state; }
    shared_state_recv(std::shared_ptr<shared_state<T, ConditionVariableType, mutex_type>>&& state) { state_ = std::move(state); }

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

    void set_on_complete(std::function<void()> on_complete) {
        state_->set_on_complete(on_complete);
    }

    std::shared_ptr<shared_state<T, ConditionVariableType, mutex_type>> state() {
        return state_;
    }
};

}}
