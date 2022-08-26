#pragma once

#include <fiber/common.hpp>


namespace il { namespace fiber { namespace impl { 
class windows_context_impl {
public:
    windows_context_impl(fiber_and_routine fiber_data, bool is_main_fiber);
    ~windows_context_impl();

    bool is_valid() const {
        return 
    }

private:
    void*               address_;
    bool                is_main_fiber_;
    fiber_and_routine   fiber_and_routine_;
};
}}}
