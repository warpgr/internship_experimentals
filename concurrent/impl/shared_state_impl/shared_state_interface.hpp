#pragma once
#include <concurrent/condition_variable.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <exception>
#include <chrono>
#include <memory>

namespace il { namespace impl {

#define have_correct_state(state) assert(nullptr != state)

template <typename T>
class shared_state {
protected:
    condition_variable<unique_lock<mutex>>              is_ready_;
    mutex                                               guard_;
    T                                                   shared_data_; // TODO: must be with allocator // shared_ptr
    std::shared_ptr<std::exception>                     ex_ptr_ = nullptr;
public:
    shared_state() { };
    shared_state(shared_state&& other) = delete;
    shared_state(shared_state& other) = delete;
    shared_state&& operator=(shared_state&& other) = delete;
    shared_state& operator=(shared_state& other) = delete;

public: // Receiver interface
    T get() {
        wait();
        if (nullptr != ex_ptr_) {
            throw ex_ptr_;
        }
        return shared_data_;
    }
    void wait() {
        unique_lock<mutex> lock(guard_);
        is_ready_.wait(lock);
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
public: // Sender interface
    void set_value(T& value) {
        unique_lock<mutex> lock(guard_);
        shared_data_ = value;
        is_ready_.notify_one();
    }
    void set_value(T&& value) {
        unique_lock<mutex> lock(guard_);
        shared_data_(std::move(value));
        is_ready_.notify_one();
    }
    void set_value(const T& value) {
        unique_lock<mutex> lock(guard_);
        shared_data_(value);
        is_ready_.notify_one();
    }
    void set_value() {
        unique_lock<mutex> lock(guard_);
        shared_data_ = T();
        is_ready_.notify_one();
    }
    void set_exception(const std::exception& ex) {
       set_exception(std::exception(ex));
    }
    void set_exception(std::exception&& ex) {
        ex_ptr_ = std::make_shared<std::exception>(std::move(ex));
        is_ready_.notify_one();
    }
};
}}

