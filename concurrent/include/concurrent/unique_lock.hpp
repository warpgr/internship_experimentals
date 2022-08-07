
#pragma once
#include <chrono>
#include <concurrent/mutex.hpp>
#include <cassert>

namespace il {

template <typename T>
concept UniqueLock = 
    Lockable<T>                          && /* defined in mutex.hpp */
    std::default_initializable<T>        &&
    std::destructible<T>                 &&
    std::move_constructible<T>           &&
    std::is_move_assignable<T>::value    &&
    !std::copy_constructible<T>          &&
    !std::is_copy_assignable<T>::value;

template <Mutex mutex_type>
class unique_lock {
    mutex_type* m_ = { nullptr };
public:
    unique_lock() = default;
    unique_lock(mutex_type& m) : m_(&m) {
        assert(m_ != nullptr);
        lock();
    }
    unique_lock&& operator=(unique_lock&& other) {
        m_(other.m_);
        other.m_ = nullptr;
        return std::move(*this);
    }

    unique_lock(unique_lock&& other) {
        m_ = (other.m_);
        other.m_ = nullptr;
    }
    unique_lock(const unique_lock& other) = delete;
    unique_lock& operator=(const unique_lock& other) = delete;
    ~unique_lock() {
        if (nullptr != m_) {
            unlock();
        }
    }

public:
    void lock();
    void unlock();
    bool try_lock();
    bool try_lock_for(const std::chrono::duration<int>& timeout_duration);
    bool try_lock_until(const std::chrono::time_point<std::chrono::system_clock>& time_point);
    void swap(unique_lock &other)noexcept;
    mutex_type* release() noexcept;
};

template <Mutex mutex_type>
void unique_lock<mutex_type>::lock() {
    assert(nullptr != m_);
    m_->lock();
}

template <Mutex mutex_type>
void unique_lock<mutex_type>::unlock() {
    assert(nullptr != m_);
    m_->unlock();
}

template <Mutex mutex_type>
bool unique_lock<mutex_type>::try_lock() {
    assert(nullptr != m_);
    return m_->try_lock();
}

template <Mutex mutex_type>
bool unique_lock<mutex_type>::try_lock_for(const std::chrono::duration<int>& timeout_duration) {
    auto lock_until_time_point = std::chrono::system_clock::now();
    lock_until_time_point += timeout_duration;
    while (std::chrono::system_clock::now() < lock_until_time_point) {
        if (try_lock()) {
            return true;
        }
    }
    return false;
}

template <Mutex mutex_type>
bool unique_lock<mutex_type>::try_lock_until(const std::chrono::time_point<std::chrono::system_clock>& time_point) {
    while (std::chrono::system_clock::now() < time_point) {
        if (try_lock()) {
            return true;
        }
    }
    return false;
}

template <Mutex mutex_type>
void unique_lock<mutex_type>::swap(unique_lock &other) noexcept {
        std::swap(m_, other.m_);
}


template <Mutex mutex_type>
mutex_type* unique_lock<mutex_type>::release() noexcept {
        mutex_type* other = m_;
        m_ = nullptr;
        return other;
}

template<Mutex mutex_type>
    inline void swap(unique_lock<mutex_type>& lhs, unique_lock<mutex_type>& rhs) noexcept {
        lhs.swap(rhs);
}

}

