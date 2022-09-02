#pragma once 
#include <future_impl/future_impl.hpp>
#include <concurrent/fiber/condition_variable.hpp>
#include <concurrent/fiber/mutex.hpp>


namespace il { namespace fiber {
template <typename T>
using future = ::il::impl::future_impl<T, condition_variable<unique_lock<mutex>>, mutex, function_executor>;

template <typename T>
using promise = ::il::impl::promise_impl<T, condition_variable<unique_lock<mutex>>, mutex, function_executor>;


}}