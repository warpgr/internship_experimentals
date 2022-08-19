#include <concurrent/mutex.hpp>

namespace il {
void mutex::lock() {
    while (captured.exchange(true,std::memory_order_acq_rel)); //synchronizes with 
}
    
void mutex::unlock() {
    captured = false;
}

bool mutex::try_lock() {
    return !(captured.exchange(true,std::memory_order_acq_rel)); //synchronizes with
}
}
