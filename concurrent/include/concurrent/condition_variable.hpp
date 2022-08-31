#pragma once 

#include <concurrent/handler.hpp>
#include <chrono>
#include <type_traits>

#include <concurrent/utility_concepts.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>

#include <synchronization_primitives/condition_variable_impl.hpp>


namespace il {


template <typename lock_type = il::unique_lock<il::mutex>>
using condition_variable = impl::condition_variable_impl<impl::ThreadBlockingHandler, lock_type>;


} // namespace il
