#pragma once


#include <fiber/common.hpp>

#define _XOPEN_SOURCE

#include <ucontext.h>
#include <cstdint>
#include <memory>

namespace il { namespace fiber { namespace impl {
class posix_context_impl {
public:
    posix_context_impl(fiber_and_routine fiber_data, bool is_main_fiber);
    ~posix_context_impl();

    bool is_valid() const {
        return is_valid_;
    }

    static void swap(posix_context_impl& from, posix_context_impl& to);

private:
    ucontext_t                 context_;
    std::unique_ptr<uint8_t[]> stack_;
    bool                       is_valid_;
    fiber_and_routine          fiber_and_routine_;
};
}}}
