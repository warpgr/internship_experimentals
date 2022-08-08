#pragma once 
#include <shared_state_impl/shared_state_recv.hpp>
#include <memory>

namespace il {
template <typename T>
future<T> create_future(std::shared_ptr<il::impl::shared_state_recv>& shared_state_ptr)

template <typename T>
class future {
    std::shared_ptr<il::impl::shared_state_recv> state_;
    friend future<T> create_future(std::shared_ptr<il::impl::shared_state_recv>& shared_state_ptr);
    // friend class promise<T>;
private:
    future(std::shared_ptr<il::impl::shared_state_recv>& shared_state_ptr);
public:
    future() = delete;
    future(const future& other) = delete;
    future& operator=(const future& other) = delete;

    future(future&& other) = default;
    future&& operator=(future&& other) = default;

public:
    void wait() {
        state_.wait();
    }

    template <typename Rep, typename Period = std::ratio<1>>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        state_.wait(duration);
    }

    template <typename Clock, typename Duration = Clock::duration>
    void wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
        state_.wait_until(timeout_time);
    }

    T get() {
        return state_.get();
    }

    bool valid() const  {
        return (nullptr != state_);
    }
};

#include <shared_state_impl/shared_state_send.hpp>


template <typename T>
class promise {
    std::shared_ptr<il::impl::shared_state_send> state_;
public:
    promise() { state_ = std::make_shared<il::impl::shared_state_send>(); }
    promise(const promise& other) = delete;
    promise(promise&& other) = default;
    promise& operator=(const promise& other) = delete;
    promise&& operator=(promise&& other) = default;

public:
    future<T> get_future() {
        return create_future(
            static_cast<std::shared_ptr<il::impl::shared_state_recv>>(state_));
    }
    void set_value(T& value) {
        state_->set_value(std::forward<T>(value));
    }
    void set_value(T&& value) {
        state_->set_value(std::forward<T>(value));
    }
    void set_value(const T& value) {
        state->set_value(std::forward<T>(value));
    }
    void set_value() {
        state_->set_value(std::forward<T>(T()));
    }
    void set_exception(std::exception* ex) {
        ex_ptr_ = ex;
        is_ready_.notify_one();
    }
};

template <typename T>
future<T> create_future(std::shared_ptr<il::impl::shared_state_recv>& shared_state_ptr, ) {
    return future<T>(shared_state_ptr);
}
}