#pragma once 

#include <atomic>
#include <chrono>
#include <type_traits>

#include <concurrent/unique_lock.hpp>

namespace il {

template <typename Func, typename... Args>
concept Predicate =
    std::regular_invocable<Func, Args...> &&
    std::convertible_to<std::invoke_result_t<Func, Args...>, bool>;

template <UniqueLock lock_type>
class condition_variable {
private:
    std::atomic<bool> flag_ = { false };
public:
    condition_variable() = default;
    condition_variable(const condition_variable&) = delete;
    condition_variable(condition_variable&&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;
    condition_variable&& operator=(condition_variable&&) = delete;
public:
    void wait(lock_type& lock);

    template <Predicate predicate_type>
    void wait(lock_type& lock, predicate_type pred) {
        while (!pred()) {
            wait(lock);
        }
    }

    bool wait_for(lock_type& lock, const std::chrono::duration<int>& timeout_duration) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now);
    }

    template <Predicate predicate_type>
    bool wait_for(lock_type& lock, const std::chrono::duration<int>& timeout_duration,
                  predicate_type pred) {
        auto now = std::chrono::system_clock::now();
        now += timeout_duration;
        return wait_until(lock, now, pred);
    }

    bool wait_until(lock_type& lock, 
              const std::chrono::time_point<std::chrono::system_clock>& time_point) {
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            is_notified = flag_.exchange(false);
            if (is_notified) { break; }
        }
        return is_notified;
    }

    template <Predicate predicate_type>
    bool wait_until(lock_type& lock, 
            const std::chrono::time_point<std::chrono::system_clock>& time_point,
                  predicate_type pred) {
        bool is_notified = false;
        while (std::chrono::system_clock::now() < time_point) {
            if (pred()) {
                is_notified = flag_.exchange(false);
                if (is_notified) { return true; }
            }
        }
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


template <UniqueLock lock_type>
void condition_variable<lock_type>::wait(lock_type& lock) {
    while (!flag_.exchange(false)) {
        lock_unlock(lock);
    }
}

} // namespace il
