#include <concurrent/mutex.hpp>


/*
synchronizes with - no instruction can be
reordered before an acquire operation
and no instruction can be reordered after a release operation;
*/ 

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
