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

fiber_ptr fiber::get_active_fiber() {
    assert(thread_local_d);
    return thread_local_d->scheduler_->get_active_fiber();
}

fiber_ptr fiber::create(const std::function<void()>& func, const std::string& name) {
    get_main_fiber();
    return create(func, name, false);
}

void fiber::yield_to(fiber_ptr fib) {
    assert(fib->is_valid() && !fib->is_active() && !fib->is_finished());
    assert(thread_local_d && thread_local_d->scheduler_->get_active_fiber());
    assert(thread_local_d->scheduler_->get_active_fiber()->is_valid() && thread_local_d->scheduler_->get_active_fiber()->is_active());
    assert(fib != thread_local_d->scheduler_->get_active_fiber());

    thread_local_d->scheduler_->on_fiber_yield_to(fib);

    impl::context_impl& from = *(thread_local_d->scheduler_->get_active_fiber()->impl_.get());
    impl::context_impl& to = *(fib->impl_.get());
    thread_local_d->scheduler_->set_active_fiber(fib);
    impl::context_impl::swap(from, to);
}

void fiber::yield() {
    assert(thread_local_d && thread_local_d->scheduler_->get_active_fiber() && thread_local_d->scheduler_);

    fiber_ptr next_fib = thread_local_d->scheduler_->next();
    if (!next_fib) {
        return;
    }

    assert(next_fib->is_valid() && !next_fib->is_finished()); // !next_fib->is_active()
    // assert(next_fib != thread_local_d->scheduler_->get_active_fiber()); // TODO: check activities

    yield_to(next_fib);
}

void fiber::set_scheduler(std::unique_ptr<scheduler> new_scheduler) {
    assert(thread_local_d);

    if (new_scheduler) {
        thread_local_d->scheduler_ = std::move(new_scheduler);

        for (fiber_ptr fib : thread_local_d->fibers_) {
            assert(fib->is_valid());
            thread_local_d->scheduler_->on_fiber_created(fib);
        }

        if (thread_local_d->scheduler_->get_active_fiber()) {
            thread_local_d->scheduler_->on_fiber_yield_to(thread_local_d->scheduler_->get_active_fiber());
        }
    }
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
    if (!new_fiber->is_valid()) {
        return nullptr;
    }
    if (is_main_fiber) {
        assert(new_fiber->is_valid() && function == nullptr && thread_local_d && thread_local_d->scheduler_->get_active_fiber() == nullptr);
        thread_local_d->scheduler_->set_active_fiber(new_fiber);
    }
    if (new_fiber->is_valid() && !is_main_fiber) {
        thread_local_d->fibers_.push_back(new_fiber);
        thread_local_d->scheduler_->on_fiber_created(new_fiber);

        // if (is_main_fiber) {
        //     thread_local_d->scheduler_->on_fiber_yield_to(new_fiber);
        // }
    }
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

    data_->scheduler_->on_fiber_finished(shared_from_this());
    data_->fibers_.erase(std::remove_if(data_->fibers_.begin(), data_->fibers_.end(), [this_shared_ptr = shared_from_this()] (const fiber_ptr fib) { return fib == this_shared_ptr; }), data_->fibers_.end());
}


}}
