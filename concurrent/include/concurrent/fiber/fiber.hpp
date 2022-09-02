#pragma once
#include <memory>
#include <functional>
#include <fiber/context_impl.hpp>
#include <concurrent/fiber/scheduller.hpp>
#include <chrono>

namespace il { namespace fiber {

class fiber : public std::enable_shared_from_this<fiber> {
public:
    static fiber_ptr get_main_fiber();
    static fiber_ptr create(const std::function<void()>& function, const std::string& name = "fiber");
    static void yield_to(fiber_ptr f);
    static void yield();
    static std::shared_ptr<scheduler> get_scheduler();
    static void wait_for_all();

    template <typename Rep, typename Period = std::ratio<1>> 
        requires std::is_arithmetic_v<Rep>
    static void this_fiber_sleep_for(const std::chrono::duration<Rep, Period>& timeout_duration);

    template <typename Clock, typename Duration = typename Clock::duration>
        requires std::chrono::is_clock_v<Clock>
    static void this_fiber_sleep_until(const std::chrono::time_point<Clock, Duration>& time_point);
public:
    fiber(const std::function<void()>& func, const std::string& name, bool is_main_fiber);

    ~fiber();

    const std::string& name() const {
        return fiber_name_;
    }

    bool is_active() const {
        return thread_local_d->scheduler_->get_active_fiber().get() == this;
    }

    bool is_finished() const {
        return is_finished_;
    }

    bool is_main_fiber() const {
        return is_main_fiber_;
    }

    bool is_valid() const {
        return impl_->is_valid();
    }

private:
    static void fiber_routine(fiber_ptr fib);
    static fiber_ptr create(const std::function<void()>& function, const std::string& name, bool is_main_fiber);
    fiber(const fiber& other) = delete;
    fiber(fiber&& other) = delete;
    fiber& operator=(const fiber& other) = delete;
    fiber& operator=(fiber&& other) = delete;

    void finish();

    struct thread_local_data {
        thread_local_data(std::shared_ptr<scheduler> initial_shceduler)
            : scheduler_(std::move(initial_shceduler)) { }

        std::shared_ptr<scheduler>      scheduler_;
    };

private:

    static thread_local std::shared_ptr<thread_local_data> thread_local_d;
    std::function<void()>                                  function_;
    std::string                                            fiber_name_;
    const bool                                             is_main_fiber_;
    bool                                                   is_finished_;
    std::shared_ptr<impl::context_impl>                    impl_;
    std::shared_ptr<thread_local_data>                     data_;
};

template <typename Rep, typename Period> 
        requires std::is_arithmetic_v<Rep>
void fiber::this_fiber_sleep_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
    auto now = std::chrono::system_clock::now();
    now += timeout_duration;
    return this_fiber_sleep_until(now);
}


template <typename Clock, typename Duration>
    requires std::chrono::is_clock_v<Clock>
void fiber::this_fiber_sleep_until(const std::chrono::time_point<Clock, Duration>& time_point) {
    while (std::chrono::system_clock::now() < time_point) {
        yield();
    }
}

}}
