#pragma once 
#include <future_impl/future_impl.hpp>
#include <concurrent/condition_variable.hpp>

namespace il {
template <typename T>
using future = impl::future_impl<T, condition_variable>;

template <typename T>
using promise = impl::promise<T, condition_variable>;

}
