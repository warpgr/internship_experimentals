#pragma once 
#include <concurrent/fiber/handler.hpp>
#include <synchronization_primitives/condition_variable_impl.hpp>
#include <concurrent/unique_lock.hpp>
#include <concurrent/fiber/mutex.hpp>

namespace il { namespace fiber {

template <typename LockType = unique_lock<mutex>>
using condition_variable =impl:: condition_variable_impl<impl::FiberBlcokingHandler, LockType>;
}}
