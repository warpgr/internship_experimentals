#include <concurrent/mutex.hpp>
#include <concurrent/containers/list.hpp>

int main() {
    il::mutex m;
    m.lock();
    m.unlock();

    il::mutex m1;
    il::lock_guard<il::mutex> l (m1);
}

