#pragma once 

#include <atomic>
#include <chrono>
#include <type_traits>

#include <concurrent/utility_concepts.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>

namespace il {

template <Lockable lock_type = il::unique_lock<il::mutex>>
class condition_variable {

    std::atomic<bool> flag_ = { false };

public:
    condition_variable() = default;
    condition_variable(const condition_variable&) = delete;
    condition_variable(condition_variable&&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;
    condition_variable&& operator=(condition_variable&&) = delete;

public:
    void wait(lock_type& lock) {
        lock.unlock();
        while (!flag_.exchange(false));
        lock.lock();
    }

    template <Predicate predicate_type>
    void wait(lock_type& lock, predicate_type pred) {
        while (!pred()) {
            wait(lock);
        }
    }

    template <typename Rep, typename Period = std::ratio<1>> 
        requires std::is_arithmetic_v<Rep>
    bool wait_for(lock_type& lock, const std::chrono::duration<Rep, Period>& timeout_duration) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now);
    }

    template <typename Rep, typename Period = std::ratio<1>, Predicate predicate_type>
        requires std::is_arithmetic_v<Rep>
    bool wait_for(lock_type& lock, const std::chrono::duration<Rep, Period>& timeout_duration,
                  predicate_type pred) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now, pred);
    }

    template <typename Clock, typename Duration = typename Clock::duration>
        requires std::chrono::is_clock_v<Clock>
    bool wait_until(lock_type& lock, const std::chrono::time_point<Clock, Duration>& time_point) {
        lock.unlock();
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            is_notified = flag_.exchange(false);
            if (is_notified) { break; }
        }
        lock.lock();
        return is_notified;
    }

    template <typename Clock, typename Duration = typename Clock::duration, Predicate predicate_type>
        requires std::chrono::is_clock_v<Clock>
    bool wait_until(lock_type& lock, const std::chrono::time_point<Clock, Duration>& time_point,
                    predicate_type pred) {
        bool is_notified = false;
        lock.unlock();
        while (std::chrono::system_clock::now() < time_point) {
            if (pred()) {
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
    inline void lock_unlock(lock_type& lock) {
        lock.lock();
        lock.unlock();
    }
};


} // namespace il
