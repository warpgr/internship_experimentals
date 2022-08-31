
#pragma once

#include <atomic>



namespace il { namespace impl {



template <typename BlockingHandlerType>
class mutex_impl {
private:
    BlockingHandlerType     handler_;
    std::atomic<bool>       captured_ { false };
public:
    mutex_impl() { }
    mutex_impl (mutex_impl&& other) = delete;
    mutex_impl&& operator=(mutex_impl&& other) = delete;
    mutex_impl(const mutex_impl& other) = delete;
    mutex_impl& operator=(const mutex_impl& other) = delete;
    void lock();
    void unlock();
    bool try_lock();
};

template <typename BlockingHandlerType>
void mutex_impl<BlockingHandlerType>::lock() {
    while (captured_.exchange(true,std::memory_order_acq_rel)) {
        handler_.handle();
    } //synchronizes with
}

template <typename BlockingHandlerType>
void mutex_impl<BlockingHandlerType>::unlock() {
    captured_ = false;
}

template <typename BlockingHandlerType>
bool mutex_impl<BlockingHandlerType>::try_lock() {
    return !(captured_.exchange(true,std::memory_order_acq_rel)); //synchronizes with
}


}}
