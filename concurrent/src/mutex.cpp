#include <concurrent/mutex.hpp>

namespace il {
void mutex::lock() {
    while (captured.exchange(true));
}
    
void mutex::unlock() {
    captured = false;
}

bool mutex::try_lock() {
    return !(captured.exchange(true));
}
}
