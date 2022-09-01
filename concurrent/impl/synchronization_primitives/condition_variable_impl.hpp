#pragma once 
#include <atomic>
#include <synchronization_primitives/mutex_impl.hpp>


namespace il { namespace impl {


template <typename BlockinHandler , Mutex lock_type>
class condition_variable_impl {
    BlockinHandler    handler_;
    std::atomic<bool> flag_     = { false };
public:
    condition_variable_impl() = default;
    condition_variable_impl(const condition_variable_impl&) = delete;
    condition_variable_impl(condition_variable_impl&&) = delete;
    condition_variable_impl& operator=(const condition_variable_impl&) = delete;
    condition_variable_impl&& operator=(condition_variable_impl&&) = delete;

public:
    void wait(lock_type& lock) {
        lock.unlock();
        // A read-modify-write operation with this memory order is both an acquire operation and a release operation.
        while (!flag_.exchange(false, std::memory_order_acq_rel));
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
            // A read-modify-write operation with this memory order is both an acquire operation and a release operation
            is_notified = flag_.exchange(false, std::memory_order_acq_rel);
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
                // A read-modify-write operation with this memory order is both an acquire operation and a release operation
                is_notified = flag_.exchange(false, std::memory_order_acq_rel);
                if (is_notified) { return true; }
            }
        }
        lock.lock();
        return false;
    }

    void notify_one() {
        // A store operation with this memory order performs the release operation: 
        // no reads or writes in the current thread can be reordered after this store.
        flag_.store(true, std::memory_order_release);
    }
    void notify_all() {
        // A store operation with this memory order performs the release operation: 
        // no reads or writes in the current thread can be reordered after this store.
        flag_.store(true, std::memory_order_release);
    }
private:
    inline void lock_unlock(lock_type& lock) {
        lock.lock();
        lock.unlock();
    }
};


}}