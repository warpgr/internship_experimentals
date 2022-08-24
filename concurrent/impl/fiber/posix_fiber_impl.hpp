#pragma once


#include <fiber/common.hpp>

#define _XOPEN_SOURCE

#include <ucontext.h>
#include <cstdint>
#include <memory>

namespace il { namespace fiber { namespace impl {
class posix_fiber_impl {
public:
    posix_fiber_impl(fiber_and_main fiber_data, bool is_main_fiber);
    ~posix_fiber_impl();

    bool is_valid() const {
        return is_valid_;
    }

    static void swap(posix_fiber_impl& from, posix_fiber_impl& to);

private:
    ucontext_t                 context_;
    std::unique_ptr<uint8_t[]> stack_;
    bool                       is_valid_;
    fiber_and_main             fiber_and_main_;
};
}}}
