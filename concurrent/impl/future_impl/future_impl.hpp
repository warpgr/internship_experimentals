#pragma once 
#include <memory>
#include <type_traits>
#include <shared_state_impl/shared_state_interface.hpp>
#include <shared_state_impl/shared_state_recv.hpp>
#include <shared_state_impl/shared_state_send.hpp>

#include <concurrent/thread_pool.hpp>

namespace il {

enum launch {
    asynchronious = 1,
    deferred = 2
};

namespace impl {


template <typename T, typename ConditionVariableType, Mutex mutex_type, typename executor_type>
class future_impl;


template <typename T, typename ConditionVariableType, Mutex mutex_type, typename executor_type>
future_impl<T, ConditionVariableType, mutex_type, executor_type> 
    create_future(std::shared_ptr<impl::shared_state<T, ConditionVariableType, mutex_type>> shared_state_ptr);


template <typename T, typename ConditionVariableType, Mutex mutex_type, typename executor_type>
class promise_impl {
    std::shared_ptr<impl::shared_state_send<T, ConditionVariableType, mutex_type>> state_;
public:
    promise_impl() { state_ = std::make_shared<impl::shared_state_send<T, ConditionVariableType, mutex_type>>(); }
    promise_impl(const promise_impl& other) = delete;
    promise_impl(promise_impl&& other) {
        state_ = std::move(other.state_);
    }
    promise_impl& operator=(const promise_impl& other) = delete;
    promise_impl&& operator=(promise_impl&& other) {
        state_ = std::move(other.state_);
        return std::move(*this);
    }

public:
    future_impl<T, ConditionVariableType, mutex_type, executor_type> get_future() {
        return create_future<T, ConditionVariableType, mutex_type, executor_type>( state_->state() );
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

template <typename T, typename ConditionVariableType, Mutex mutex_type, typename executor_type>
class future_impl {
    std::shared_ptr<impl::shared_state_recv<T, ConditionVariableType, mutex_type>> state_;
private:
    friend 
        future_impl<T, ConditionVariableType, mutex_type, executor_type> 
            create_future<T, ConditionVariableType, mutex_type, executor_type>(std::shared_ptr<impl::shared_state<T, ConditionVariableType, mutex_type>> shared_state_ptr);
private:
    future_impl(std::shared_ptr<impl::shared_state<T, ConditionVariableType, mutex_type>> shared_state_ptr) {
        state_ = std::make_shared<impl::shared_state_recv<T, ConditionVariableType, mutex_type>>( std::move( shared_state_ptr ) );
    }
public:
    future_impl() = delete;
    future_impl(const future_impl& other) = delete;
    future_impl& operator=(const future_impl& other) = delete;

    future_impl(future_impl&& other) {
        state_ = std::move(other.state_);
    }
    future_impl&& operator=(future_impl&& other) {
        return future_impl<T, ConditionVariableType, mutex_type, executor_type>(std::move(other.state_));
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
    auto then(Func&& func, const launch& launch_type = launch::asynchronious)
        -> future_impl<decltype(func(T())), ConditionVariableType, mutex_type, executor_type> {
        auto prom = std::make_shared<promise_impl<decltype(func(T())), ConditionVariableType, mutex_type, executor_type>>();
        std::function<void()> on_complete =
            [&, prom_type = prom] () {
                T value = get();
                prom_type->set_value(func(std::forward<T>(value)));
            };
        switch (launch_type) {
            case launch::deferred: {
                state_->set_on_complete(on_complete);
                break;
            }
            case launch::asynchronious:
            default: {
                default_tp<executor_type>().put_task(std::move(on_complete));
                break;
            }
        }
        return prom->get_future();
    }
};


template <typename T, typename ConditionVariableType, Mutex mutex_type, typename executor_type>
future_impl<T, ConditionVariableType, mutex_type, executor_type>
create_future(std::shared_ptr<impl::shared_state<T, ConditionVariableType, mutex_type>> shared_state_ptr) {
    return future_impl<T, ConditionVariableType, mutex_type, executor_type>(shared_state_ptr);
}

}}
