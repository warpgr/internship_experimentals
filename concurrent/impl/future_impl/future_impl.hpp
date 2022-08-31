#pragma once 
#include <memory>
#include <iostream>
#include <type_traits>

#include <shared_state_impl/shared_state_recv.hpp>
#include <shared_state_impl/shared_state_send.hpp>

namespace il { namespace impl {

template <typename T, typename ConditionVariableType>
class future_impl;

template <typename T, typename ConditionVariableType>
future_impl<T, ConditionVariableType> create_future(std::shared_ptr<impl::shared_state<T, ConditionVariableType>> shared_state_ptr);

template <typename T, typename ConditionVariableType>
class future_impl {
    std::shared_ptr<impl::shared_state_recv<T, ConditionVariableType>> state_;
private:
    friend future_impl<T, ConditionVariableType> create_future<T>(std::shared_ptr<impl::shared_state<T, ConditionVariableType>> shared_state_ptr);
private:
    future_impl(std::shared_ptr<impl::shared_state<T, ConditionVariableType>> shared_state_ptr) {
        state_ = std::make_shared<impl::shared_state_recv<T, ConditionVariableType>>( std::move( shared_state_ptr ) );
    }
public:
    future_impl() = delete;
    future_impl(const future_impl& other) = delete;
    future_impl& operator=(const future_impl& other) = delete;

    future_impl(future_impl&& other) {
        state_ = std::move(other.state_);
    }
    future_impl&& operator=(future_impl&& other) {
        return future_impl<T>(std::move(other.state_));
    }
public:
    void wait() {
        state_->wait();
    }
    template <typename Rep, typename Period = std::ratio<1>>
        requires std::is_arithmetic_v<Rep>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        state_->wait_for(duration);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
        requires std::chrono::is_clock_v<Clock>
    void wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
        state_->wait_until(timeout_time);
    }
    T get() {
        return state_->get();
    }
    bool valid() const  {
        return (nullptr != state_);
    }
    template <typename Func>
    auto then(Func&& func, const launch& launch_type = launch::asynchronious) ->
        future_impl<decltype(func(T()))> {
        return state_->then(std::forward<Func>(func), launch_type);
    }
};

template <typename T, typename ConditionVariableType>
class promise {
    std::shared_ptr<impl::shared_state_send<T, ConditionVariableType>> state_;
public:
    promise() { state_ = std::make_shared<impl::shared_state_send<T, ConditionVariableType>>(); }
    promise(const promise& other) = delete;
    promise(promise&& other) {
        state_ = std::move(other.state_);
    }
    promise& operator=(const promise& other) = delete;
    promise&& operator=(promise&& other) {
        state_ = std::move(other.state_);
        return std::move(*this);
    }

public:
    future_impl<T> get_future() {
        return create_future( state_->state() );
    }
    void set_value(T& value) {
        state_->set_value(std::forward<T>(value));
    }
    void set_value(T&& value) {
        state_->set_value(std::forward<T>(value));
    }
    void set_value(const T& value) {
        state_->set_value(std::forward<T>(value));
    }
    void set_value() {
        state_->set_value(std::forward<T>(T()));
    }
    void set_exception(std::exception* ex) {
        state_->set_exception(ex);
    }

    void set_callback(std::function<void()> callback) {
        state_->set_callback(callback);
    }
};

template <typename T, typename ConditionVariableType>
future_impl<T, ConditionVariableType> create_future(std::shared_ptr<impl::shared_state<T, ConditionVariableType>> shared_state_ptr) {
    return future_impl<T>(shared_state_ptr);
}
}}
