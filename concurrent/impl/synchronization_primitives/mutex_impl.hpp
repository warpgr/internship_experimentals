
#pragma once

#include <atomic>
#include <concepts>


namespace il {

template <typename T>
concept Lockable = requires(T m) {
    { m.lock()     };
    { m.unlock()   };
    { m.try_lock() };
};

template <typename T>
concept Mutex = 
    Lockable<T>                   &&
    std::default_initializable<T> &&
    std::destructible<T>          &&
    !std::movable<T>              &&
    !std::copyable<T>;

namespace impl {


template <typename BlockingHandlerType>
class mutex_impl {
private:
    std::atomic<bool>       captured_  = { false };
public:
    mutex_impl() = default;
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
        BlockingHandlerType::handle();
    } //synchronizes with
}

template <typename BlockingHandlerType>
void mutex_impl<BlockingHandlerType>::unlock() {
    captured_ = false;
}

template <typename BlockingHandlerType>
bool mutex_impl<BlockingHandlerType>::try_lock() {
    return !(captured_.exchange(true, std::memory_order_acq_rel)); //synchronizes with
}

}}
