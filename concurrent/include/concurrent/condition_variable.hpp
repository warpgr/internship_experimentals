#pragma once 

#include <atomic>
#include <chrono>
// #include <concepts>

namespace il {

// template <typename Func, typename... Args>
// concept Predicate = std::regular_invocable<Func, Args...> &&
//                     std::convertible_to<std::invoke_result_t<Func, Args...>, bool>;

template <typename LockType>
class condition_variable {

    std::atomic<bool> flag_ = { false };

public:
    condition_variable() = default;
    condition_variable(const condition_variable&) = delete;
    condition_variable(condition_variable&&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;
    condition_variable&& operator=(condition_variable&&) = delete;

public:

    void wait(LockType& lock);
    // template<Predicate PredType>
    template <typename Func, typename... Args>
    void wait(LockType& lock, Func pred, Args... args) {
        while (!pred(std::forward(args)...)) {
            wait(lock);
        }
    }

    bool wait_for(LockType& lock, const std::chrono::duration<int>& timeout_duration) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now);
    }
    template <typename Func, typename... Args>
    bool wait_for(LockType& lock, const std::chrono::duration<int>& timeout_duration,
                  Func func, Args... args) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now, func, std::forward(args)...);
    }

    bool wait_until(LockType& lock, const std::chrono::time_point<std::chrono::system_clock>& time_point) {
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            is_notified = flag_.exchange(false);
            if (is_notified) { break; }
        }
        return is_notified;
    }
    template <typename Func, typename... Args>
    bool wait_until(LockType& lock, const std::chrono::time_point<std::chrono::system_clock>& time_point,
                    Func func, Args... args) {
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            if (func(std::forward(args)...)) {
                is_notified = flag_.exchange(false);
                if (is_notified) { return true; }
            }
        }
        return false;
    }
    void notify_one();
    void notify_all();
private:
    inline void lock_unlock(LockType& lock) {
        lock.lock();
        lock.unlock();
    }
};


template <typename LockType>
void condition_variable<LockType>::wait(LockType& lock) {
    while (!flag_.exchange(false)) {
        lock_unlock(lock);
    }
}


// template <typename LockType, Predicate PredType>
// void condition_variable<LockType, PredType>::wait(LockType& lock, PredType pred) {
// }


// template <typename LockType>
// void condition_variable<LockType>::wait_for(LockType& lock, const std::chrono::duration<int>& timeout_duration) 

// template <typename LockType>
// void condition_variable<LockType>::wait_until(LockType& lock, const std::chrono::time_point<std::chrono::system_clock>& time_point) 

template <typename LockType>
void condition_variable<LockType>::notify_one() {
    flag_.store(true);
}

template <typename LockType>
void condition_variable<LockType>::notify_all() {
    flag_.store(true);
}


} // namespace il