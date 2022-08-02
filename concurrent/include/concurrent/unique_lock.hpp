
#pragma once
#include <chrono>
#include <concurrent/mutex.hpp>
#include <cassert>

namespace il {


template <typename MutexType = mutex>
class unique_lock {
    MutexType* m_ = { nullptr };
public:

    unique_lock() = default;
    unique_lock(MutexType& m) : m_(&m) {
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
};

template <typename MutexType>
void unique_lock<MutexType>::lock() {
    assert(nullptr != m_);
    m_->lock();
}

template <typename MutexType>
void unique_lock<MutexType>::unlock() {
    assert(nullptr != m_);
    m_->unlock();
}

template <typename MutexType>
bool unique_lock<MutexType>::try_lock() {
    assert(nullptr != m_);
    return m_->try_lock();
}

template <typename MutexType>
bool unique_lock<MutexType>::try_lock_for(const std::chrono::duration<int>& timeout_duration) {
    auto lock_until_time_point = std::chrono::system_clock::now();
    lock_until_time_point += timeout_duration;
    while (std::chrono::system_clock::now() < lock_until_time_point) {
        if (try_lock()) {
            return true;
        }
    }
    return false;
}

template <typename MutexType>
bool unique_lock<MutexType>::try_lock_until(const std::chrono::time_point<std::chrono::system_clock>& time_point) {
    while (std::chrono::system_clock::now() < time_point) {
        if (try_lock()) {
            return true;
        }
    }
    return false;
}




}