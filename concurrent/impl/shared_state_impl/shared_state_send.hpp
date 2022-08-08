#pragma once 
#include <shared_state_impl/shared_state_interface.hpp>


namespace il { namespace impl {

template <typename T>
class shared_state_send : protected il::impl::shared_state<T> {
public:
    void set_value(T& value) {
        il::unique_lock<il::mutex> lock(guard_);
        shared_data_ = value;
        is_ready_.notify_one();
    }
    void set_value(T&& value) {
        il::unique_lock<il::mutex> lock(guard_);
        shared_data_(std::move(value));
        is_ready_.notify_one();
    }
    void set_value(const T& value) {
        il::unique_lock<il::mutex> lock(guard_);
        shared_data_(value);
        is_ready_.notify_one();
    }
    void set_value() {
        il::unique_lock<il::mutex> lock(guard_);
        shared_data_ = T();
        is_ready_.notify_one();
    }
    void set_exception(std::exception* ex) {
        ex_ptr_ = ex;
        is_ready_.notify_one();
    }
    ~shared_state_send() = default;
};
}}