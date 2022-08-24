#include <fiber/posix_fiber_impl.hpp>

#include <sys/resource.h>

#include <cassert>
#include <cstddef>

namespace il { namespace fiber { namespace impl {
std::size_t get_stack_size() {
    static std::size_t stack_size = 0;

    if (stack_size == 0) {
        rlimit limit = {};
        getrlimit(RLIMIT_STACK, &limit);
        stack_size = static_cast<std::size_t>(limit.rlim_cur);
    }

    return stack_size;
}

void fiber_start(fiber_and_main* fib_and_main) {
    fib_and_main->main_function(fib_and_main->fib);
}

posix_fiber_impl::posix_fiber_impl(fiber_and_main fib_data, bool is_main_fib)
    : context_{}, stack_(nullptr), is_valid_(true), fiber_and_main_(fib_data) {
        assert(fiber_and_main_.fib && fiber_and_main_.main_function);

        is_valid_ = (getcontext(&context_) == 0);

        if (is_valid() && !is_main_fib) {
            std::size_t stack_size = get_stack_size();
            stack_ = std::unique_ptr<uint8_t[]>(new uint8_t[stack_size]);

            context_.uc_stack.ss_sp = stack_.get();
            context_.uc_stack.ss_size = stack_size;
            context_.uc_link = nullptr;

            makecontext(&context_, reinterpret_cast<void(*)()>(&fiber_start), 1, &fiber_and_main_);
        }
    }
posix_fiber_impl::~posix_fiber_impl() { }

void posix_fiber_impl::swap(posix_fiber_impl& from, posix_fiber_impl& to) {
   assert(from.is_valid() && to.is_valid());
   bool success = (swapcontext(&from.context_, &to.context_) == 0);
   assert(success);
}

}}}