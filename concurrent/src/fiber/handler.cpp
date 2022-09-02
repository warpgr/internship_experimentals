#include <concurrent/fiber/handler.hpp>



namespace il { namespace fiber { namespace impl {
    void FiberBlcokingHandler::handle() {
        fiber::yield();
    }
}}}