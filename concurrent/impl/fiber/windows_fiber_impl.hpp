#pragma once

#include <fiber/common.hpp>


namespace il { namespace fiber { namespace impl { 
class windows_fiber_impl {
public:
    windows_fiber_impl(fiber_and_main fiber_data, bool is_main_fiber);
    ~windows_fiber_impl();

    bool is_valid() const {
        return 
    }

private:
    void*           address_;
    bool            is_main_fiber_;
    fiber_and_main  fiber_and_main_;
}
}}}
