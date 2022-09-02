#pragma once 
#include <concurrent/fiber/fiber.hpp>

namespace il { namespace fiber { namespace impl {
struct FiberBlcokingHandler {
    static void handle();
};

}}}