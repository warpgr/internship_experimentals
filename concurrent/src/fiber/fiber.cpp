// #include <concurrent/fiber/fiber.hpp>

#include <algorithm>
#include <cassert>
#include  <concurrent/fiber/scheduller.hpp>
#include <concurrent/fiber/round_robin_scheduller.hpp>

namespace il { namespace fiber {

thread_local std::shared_ptr<fiber::thread_local_data> fiber::thread_local_d(new fiber::thread_local_data(std::make_unique<round_robin_scheduler>()));


fiber_ptr fiber::get_main_fiber() {
    static thread_local fiber_ptr main_fiber = create(nullptr, "main_fiber", true);
    assert(main_fiber->is_valid() && !main_fiber->is_finished() && main_fiber->is_main_fiber());

    return main_fiber;
}

fiber_ptr fiber::create(const std::function<void()>& func, const std::string& name) {
    get_main_fiber();
    return create(func, name, false);
}

void fiber::yield_to(fiber_ptr fib) {
    assert(fib->is_valid() && !fib->is_active() && !fib->is_finished());
    if (thread_local_d->scheduler_->get_active_fiber() == fib) { return; }

    impl::context_impl& from = *(thread_local_d->scheduler_->get_active_fiber()->impl_.get());
    impl::context_impl& to = *(fib->impl_.get());
    thread_local_d->scheduler_->on_fiber_yield_to(fib);
    impl::context_impl::swap(from, to);
}

void fiber::yield() {
    fiber_ptr next_fib = thread_local_d->scheduler_->next();
    if (!next_fib) { return; }
    yield_to(next_fib);
}

void fiber::fiber_routine(fiber_ptr fib) {
    assert(fib && fib->is_valid() && fib->is_active() && !fib->is_finished() && !fib->is_main_fiber() && fib->function_ != nullptr);

    fib->function_();

    fib->finish();

    fib->yield();
    fib->yield_to(get_main_fiber());

    assert(false);
}

fiber_ptr fiber::create(const std::function<void()>& function, const std::string& name, bool is_main_fiber) {
    fiber_ptr new_fiber = std::make_shared<fiber>(function, name, is_main_fiber);
    new_fiber->impl_ = std::make_shared<impl::context_impl>(impl::fiber_and_routine(new_fiber, &fiber::fiber_routine), is_main_fiber);
    if (!new_fiber->is_valid()) { return nullptr; }
    thread_local_d->scheduler_->on_fiber_created(new_fiber);
    return new_fiber;
}


fiber::fiber(const std::function<void()>& func, const std::string& name, bool is_main_fiber)
    : function_(func)
    , fiber_name_(name)
    , is_main_fiber_(is_main_fiber)
    , is_finished_(false) {
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
    fiber_ptr fib = shared_from_this();
}

}}
