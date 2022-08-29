#pragma once 
#include <memory>

namespace il { namespace fiber {
class fiber;
typedef std::shared_ptr<fiber> fiber_ptr;

class scheduler {
public:
    virtual ~scheduler() = default;
    virtual fiber_ptr next() = 0;

    virtual void on_fiber_created(fiber_ptr fib) = 0;
    virtual void on_fiber_finished(fiber_ptr fib) = 0;
    virtual void on_fiber_yield_to(fiber_ptr fib) = 0;
    virtual bool is_not_empty_queue() = 0;
    virtual fiber_ptr get_active_fiber() = 0;
};
}}
