#include <mutex.hpp>

void lib::mutex::lock() {
    while (captured.exchange(true));
}
    
void lib::mutex::unlock() {
    captured = false;
}
