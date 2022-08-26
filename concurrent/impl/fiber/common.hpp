#pragma once
#include <memory>

namespace il { namespace fiber {
class fiber;
typedef std::shared_ptr<fiber> fiber_ptr;
namespace impl {

using fiber_routine_function = void(*)(fiber_ptr);

struct fiber_and_routine {
    fiber_ptr               fiber_;
    fiber_routine_function  routine_function_;

    fiber_and_routine(fiber_ptr f, fiber_routine_function main_func)
        : fiber_(f), routine_function_(main_func) { }
};
}}}
