#include <concurrent/fiber/windows_context_impl.hpp>

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace il { namespace fiber { namespace impl {

windows_context_impl::windows_context_impl(fiber_and_main fiber_data, bool is_main_fiber) {
    : address_(nullptr), is_main_fiber_(is_main_fiber), fiber_and_main_(fiber_data) {
        assert(fiber_and_main_.fib && fiber_and_main_.main_function);

        if (is_main_fiber) {
            address_ = convertThreadToFiber(this);
            assert(address_);
        }
        else {
            address_ = CreateFiber(0, [] (LPVOID param) {
                fiber_and_main* fib_and_main = reinterpret_cast<fiber_and_main*>(param);
                assert(fib_and_main);
                fiber_and_main->main_function(fiber_and_main->fib);
            }, &fiber_and_main_);
        }
    }
}

windows_context_impl::~windows_context_impl() {
    if (is_main_fiber_) {
        assert(is_valid());
        bool success = ConvertFiberToThread() != 0;
        assert(success);
    }
    else if (address_) {
        DeleteFiber(address_);
    }
}

void windows_context_impl::swap(windows_context_impl& from, windows_context_impl& to) {
    assert(from.is_valid() && to.is_valid() && GetCurrentFiber() == from.address_ && GetCurrentFiber() != to.address_);
    SwitchToFiber(to.address_)
}

}}}
