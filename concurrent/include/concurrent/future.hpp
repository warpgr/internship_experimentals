#pragma once 
#include <shared_state_impl/shared_state_recv.hpp>
#include <shared_state_impl/shared_state_send.hpp>
#include <memory>
#include <iostream>

namespace il {

template <typename T>
class future;
template <typename T>
future<T> create_future(std::shared_ptr<impl::shared_state<T>> shared_state_ptr);

template <typename T>
class future {
    std::shared_ptr<impl::shared_state_recv<T>> state_;
private:
    friend future<T> create_future<T>(std::shared_ptr<impl::shared_state<T>> shared_state_ptr);
private:
    future(std::shared_ptr<impl::shared_state<T>> shared_state_ptr) {
        state_ = std::make_shared<impl::shared_state_recv<T>>( std::move( shared_state_ptr ) );
    }
public:
    future() = delete;
    future(const future& other) = delete;
    future& operator=(const future& other) = delete;

    future(future&& other) {
        state_ = std::move(other.state_);
    }
    future&& operator=(future&& other) {
        return future<T>(std::move(other.state_));
    }
public:
    void wait() {
        state_->wait();
    }
    template <typename Rep, typename Period = std::ratio<1>>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        state_->wait_for(duration);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
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
        future<decltype(func(T()))> {
        return state_->then(std::forward<Func>(func), launch_type);
    }
};

template <typename T>
class promise {
    std::shared_ptr<impl::shared_state_send<T>> state_;
public:
    promise() { state_ = std::make_shared<impl::shared_state_send<T>>(); }
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
    future<T> get_future() {
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

template <typename T>
future<T> create_future(std::shared_ptr<impl::shared_state<T>> shared_state_ptr) {
    return future<T>(shared_state_ptr);
}
}