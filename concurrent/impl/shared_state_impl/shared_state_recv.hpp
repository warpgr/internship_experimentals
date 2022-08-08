#pragma once

#include <shared_state_impl/shared_state_interface.hpp>

namespace il { namespace impl {

template<typename T>
class shared_state_recv : protected il::impl::shared_state<T> {
public:
    T get() {
        wait();
        if (nullptr != ex_ptr_) {
            throw *ex_ptr_;
        }
        return shared_data_;
    }
    void wait() {
        il::unique_lock<il::mutex> lock(guard_);
        is_ready_.wait(lock);
    }
    template <typename Rep, typename Period = std::ratio<1>>
    void wait_for(const std::chrono::duration<Rep, Period>& duration) {
        il::unique_lock<il::mutex> lock(guard_);
        is_ready_.wait_for(lock, duration);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
    void wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
        il::unique_lock<il::mutex> lock(guard_);
        is_ready_.wait_until(lock, timeout_time);
    }
    ~shared_state_recv() = default;
};

}}