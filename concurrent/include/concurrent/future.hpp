#pragma once 
#include <future_impl/future_impl.hpp>
#include <concurrent/condition_variable.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>

namespace il {
template <typename T>
using future = impl::future_impl<T, condition_variable<unique_lock<mutex>>, mutex>;

template <typename T>
using promise = impl::promise_impl<T, condition_variable<unique_lock<mutex>>, mutex>;

}
