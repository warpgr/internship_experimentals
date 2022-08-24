#pragma once 

#include <concurrent/fiber/fiber.hpp>
#include <concurrent/containers/mpmc_queue.hpp>
#include <concurrent/unique_lock.hpp>
#include <concurrent/mutex.hpp>

namespace il { namespace fiber {
class scheduler {
public:
    virtual ~scheduler() = default;
    virtual fiber* next(fiber* last_scheduled_fiber) = 0;

    virtual void on_fiber_created(fiber* fib) = 0;
    virtual void on_fiber_finished(fiber* fib) = 0;
    virtual void on_fiber_yield_to(fiber* fib) = 0;
};

class round_robin_scheduler : public scheduler {
public:
    round_robin_scheduler()
        : scheduler() { }

    virtual ~round_robin_scheduler() = default;

    virtual fiber* next(fiber* last_scheduled_fiber) override {
        fiber* next_fiber = fiber_queue_.pop();
        if (last_scheduled_fiber) {
            fiber_queue_.push(last_scheduled_fiber);
        }
        return next_fiber;
    }

    virtual void on_fiber_created(fiber* fib) override {
        assert(fib && scheduled_fiber_ != fib && fiber_queue_.contains(fib));
        fiber_queue_.push(fib);
    }

    virtual void on_fiber_finished(fiber* fib) override {
        assert(fib && (fib == scheduled_fiber_ || fiber_queue_.contains(fib)));
        fiber_queue_.erase(fib);
    }

    virtual void on_fiber_yield_to(fiber* fib) override {
        assert(fib && (fib == scheduled_fiber_ || fiber_queue_.contains(fib)));
        while (fib != scheduled_fiber_) {
            next();
        }
    }
private:
    mpmc_queue<fiber*>  fiber_queue_;
};



}}