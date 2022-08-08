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

    void wait(LockType& lock) {
        lock.unlock();
        while (!flag_.exchange(false));
        lock.lock();
    }
    template <typename Func, typename... Args>
    void wait(LockType& lock, Func pred, Args... args) {
        while (!pred(std::forward(args)...)) {
            wait(lock);
        }
    }

    template <typename Rep, typename Period = std::ratio<1>> 
    bool wait_for(LockType& lock, const std::chrono::duration<Rep, Period>& timeout_duration) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now);
    }
    template <typename Rep, typename Period = std::ratio<1>, typename Func, typename... Args>
    bool wait_for(LockType& lock, const std::chrono::duration<Rep, Period>& timeout_duration,
                  Func&& func, Args&&... args) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now, func, std::forward<Args>(args)...);
    }
    template <typename Clock, typename Duration = typename Clock::duration>
    bool wait_until(LockType& lock, const std::chrono::time_point<Clock, Duration>& time_point) {
        lock.unlock();
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            is_notified = flag_.exchange(false);
            if (is_notified) { break; }
        }
        lock.lock();
        return is_notified;
    }
    template <typename Clock, typename Duration = typename Clock::duration,
              typename Func, typename... Args>
    bool wait_until(LockType& lock, const std::chrono::time_point<Clock, Duration>& time_point,
                    Func&& func, Args&&... args) {
        bool is_notified = false;
        lock.unlock();
        while (std::chrono::system_clock::now() < time_point) {
            if (func(std::forward<Args>(args)...)) {
                is_notified = flag_.exchange(false);
                if (is_notified) { return true; }
            }
        }
        lock.lock();
        return false;
    }
    void notify_one() {
        flag_.store(true);
    }
    void notify_all() {
        flag_.store(true);
    }
private:
    inline void lock_unlock(LockType& lock) {
        lock.lock();
        lock.unlock();
    }
};


} // namespace il