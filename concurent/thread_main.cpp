#include <mutex.hpp>
/* #include <list.hpp> */

int main() {
    lib::mutex m;
    m.lock();
    m.unlock();

    lib::mutex m1;
    lib::lock_guard<lib::mutex> l (m1);
}

