#pragma once 
#include <concurrent/fiber/fiber.hpp>
#include <concurrent/fiber/handler.hpp>
#include <synchronization_primitives/mutex_impl.hpp>

namespace il { namespace fiber {
using mutex =  ::il::impl::mutex_impl<impl::FiberBlcokingHandler>;
}}