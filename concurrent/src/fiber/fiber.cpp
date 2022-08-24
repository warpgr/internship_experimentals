#include <concurrent/fiber/fiber.hpp>
// #include <concurrent/fiber/round_robin_scheduler.hpp>

#include <algorithm>
#include <cassert>

namespace il { namespace fiber {


fiber& fiber::get_main_fiber() {
    static thread_local fiber main_fiber(nullptr, "main fiber", true);
    assert(main_fiber.is_valid() && !main_fiber.is_finished() && main_fiber.is_main_fiber());
    return main_fiber;
}

std::unique_ptr<fiber> fiber::create(const std::function<void()>& func, const std::string& name) {
    get_main_fiber();
    std::unique_ptr<fiber> new_fiber(new fiber(func, name, false));
    if (!new_fiber->is_valid()) {
        return nullptr;
    }
    return new_fiber;
}

void fiber::yield_to(fiber& from, fiber& to) {
    assert(&from != &to);
    assert(&from != nullptr);
    assert(&to != nullptr);
    assert(to.is_valid() && !to.is_active() && !to.is_finished());
    assert(from.is_valid() && from.is_active());

    // thread_local_d->scheduler_->on_fiber_yield_to(&fib); call  from thread_pool
    impl::fiber_impl::swap(from.impl_, to.impl_);
}

// void fiber::yield() {
//     assert(thread_local_d && thread_local_d->active_fiber_ && thread_local_d->scheduler_);

//     fiber* next_fib = thread_local_d->scheduler_->next();
//     if (!next_fib) {
//         return;
//     }

//     assert(next_fib->is_valid() && !next_fib->is_active() && !next_fib->is_finished());
//     assert(next_fib != thread_local_d->active_fiber_);

//     yield_to(*next_fib);
// }

// void fiber::set_scheduler(std::unique_ptr<scheduler> new_scheduler) {
//     assert(thread_local_d);

//     if (new_scheduler) {
//         thread_local_d->scheduler_ = std::move(new_scheduler);

//         for (fiber* fib : thread_local_d->fibers_) {
//             assert(fib->is_valid());
//             thread_local_d->scheduler_->on_fiber_created(fib);
//         }

//         if (thread_local_d->active_fiber_) {
//             thread_local_d->scheduler_->on_fiber_yield_to(thread_local_d->active_fiber_);
//         }
//     }
// }

void fiber::fiber_main(fiber* fib) {
    assert(fib && fib->is_valid() && fib->is_active() && !fib->is_finished() && !fib->is_main_fiber() && fib->function_ != nullptr);

    fib->function_();

    fib->finish();

    // fib->yield();
    // fib->yield_to(this , get_main_fiber());

    assert(false);
}


fiber::fiber(const std::function<void()>& func, const std::string& name, bool is_main_fib)
    : function_(func), fiber_name_(name), is_main_fiber_(is_main_fib), is_finished_(false), 
    impl_(
        impl::fiber_and_main(this, &fiber::fiber_main),
        is_main_fib) {
    
    // if (is_main_fib) {
    //     assert(is_valid() && func == nullptr && thread_local_d && thread_local_d->active_fiber_ == nullptr);
    //     thread_local_d->active_fiber_ = this;
    // }

    // data_ = thread_local_d;
    // assert(data_);

    // if (is_valid()) {
    //     data_->fibers_.push_back(this);
    //     data_->scheduler_->on_fiber_created(this);

    //     if (is_main_fib) {
    //         data_->scheduler_->on_fiber_yield_to(this);
    //     }
    // }
}

fiber::~fiber() {
    assert(is_active() == is_main_fiber());

    if (is_valid() && !is_finished()) {
        finish();
    }
}

void fiber::finish() {
    assert(is_valid() && !is_finished());

    is_finished_ = true;

    // data_->scheduler_->on_fiber_finished(this);
    // data_->fibers_.erase(std::remove_if(data_->fibers_.begin(), data_->fibers_.end(), [this] (const fiber* fib) { return fib == this; }), data_->fibers_.end());
}
}}
