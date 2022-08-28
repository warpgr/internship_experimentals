

#pragma once
#include <concurrent/fiber/scheduller.hpp>
#include <concurrent/fiber/fiber.hpp>
#include <concurrent/unique_lock.hpp>
#include <concurrent/mutex.hpp>
#include <map>
#include <thread>
#include <deque>


namespace il { namespace fiber {

class round_robin_scheduler : public scheduler {
public:
    round_robin_scheduler()
        : scheduler() { }
    
    virtual ~round_robin_scheduler() = default;

    virtual fiber_ptr next() override {
        if (fiber_queue_.empty()) {
            return nullptr;
        }
        // fiber_ptr active_fiber_for_this_thread = get_current_schedulled_fiber_for_this_thread(true);
        fiber_ptr next_fiber_for_this_thread = take_next_fiber();

        set_new_schedulled_fiber_for_this_thread(next_fiber_for_this_thread, false);

        return next_fiber_for_this_thread;
    }

    virtual void on_fiber_created(fiber_ptr fib) override {
        // fiber_ptr current_thrad_schedulled_fiber = get_current_schedulled_fiber_for_this_thread();
        assert(fib && std::find(fiber_queue_.begin(), fiber_queue_.end(), fib) == fiber_queue_.end());
        add_fiber(fib);
    }

    virtual void on_fiber_finished(fiber_ptr fib) override {
        fiber_ptr active_fiber_for_this_thread = get_current_schedulled_fiber_for_this_thread(true);
        remove_fiber_from_queue_if_contains(fib);
    }

    virtual void on_fiber_yield_to(fiber_ptr fib) override {
        fiber_ptr current_thrad_active_fiber    = get_current_schedulled_fiber_for_this_thread(true);
        fiber_ptr current_thrad_scheduled_fiber = get_current_schedulled_fiber_for_this_thread(false);
        if (!current_thrad_active_fiber->is_finished()) {
            add_fiber(current_thrad_active_fiber);
        }
        if (fib && fib != current_thrad_scheduled_fiber) {
            set_new_schedulled_fiber_for_this_thread(fib, true);
            remove_fiber_from_queue_if_contains(fib);
            add_fiber(current_thrad_scheduled_fiber);
        }
        else {
            set_new_schedulled_fiber_for_this_thread(current_thrad_scheduled_fiber, true);
        }
    }

    virtual fiber_ptr get_active_fiber() override {
        return get_current_schedulled_fiber_for_this_thread(true);
    }

    virtual void set_active_fiber(fiber_ptr fib) override {
        set_new_schedulled_fiber_for_this_thread(fib, true);
    }

private:
    mutex                                       fiber_queue_guard_;
    std::deque<fiber_ptr>                       fiber_queue_;

    mutex                                       scheduled_fibers_guard_;
    std::map<std::thread::id, 
            std::pair<fiber_ptr, fiber_ptr>>    scheduled_fibers_;

private:

    fiber_ptr get_current_schedulled_fiber_for_this_thread(bool is_active) {
        unique_lock<mutex> scheduled_fibers_lock(scheduled_fibers_guard_);
        if (scheduled_fibers_.size() == 0) { return nullptr; }

        auto found_fiber = scheduled_fibers_.find(std::this_thread::get_id());
        if (scheduled_fibers_.end() == found_fiber) { return nullptr; }
        return  (is_active) ? found_fiber->second.first : found_fiber->second.second;
    }
    void set_new_schedulled_fiber_for_this_thread(fiber_ptr fib, bool is_active) {
        unique_lock<mutex> scheduled_fibers_lock(scheduled_fibers_guard_);
        if (scheduled_fibers_.find(std::this_thread::get_id()) == scheduled_fibers_.end()) {
            scheduled_fibers_.insert(
            std::make_pair<std::thread::id, std::pair<fiber_ptr, fiber_ptr>>(
                std::move(std::this_thread::get_id()), std::make_pair<fiber_ptr, fiber_ptr>(std::move(fib), nullptr)));
        }
        else if (is_active) {
            scheduled_fibers_.at(std::this_thread::get_id()).first = fib;
        }
        else {
            scheduled_fibers_.at(std::this_thread::get_id()).second = fib;
        }
    }

    fiber_ptr take_next_fiber() {
        unique_lock<mutex> lock(fiber_queue_guard_);
        fiber_ptr next_fiber = std::move(fiber_queue_.front());
        fiber_queue_.pop_front();
        return next_fiber;
    }

    void add_fiber(fiber_ptr fib) {
        unique_lock<mutex> lock(fiber_queue_guard_);
        fiber_queue_.push_back(fib);
    }

    void remove_fiber_from_queue_if_contains(fiber_ptr fib) {
        unique_lock<mutex> lock(fiber_queue_guard_);
        fiber_queue_.erase(std::remove_if(fiber_queue_.begin(), fiber_queue_.end(), [fib] (fiber_ptr el) { return el == fib; }));
    }

};

}}