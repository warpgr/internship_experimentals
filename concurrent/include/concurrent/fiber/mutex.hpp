#pragma once 
#include <concurrent/fiber/fiber.hpp>
#include <concurrent/fiber/handler.hpp>

namespace il { namespace fiber {
using mutex =  impl::mutex_impl<impl::FiberBlcokingHandler>;
}}