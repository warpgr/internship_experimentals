#include <concurrent/fiber/thread_pool.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <sstream>
#include <iostream>


int main() {
    int num = 0;
    il::mutex num_guard;
    {
        il::fiber::thread_pool<il::fiber::fiber_executor> tp;

        for (int i = 0; i < 1000; ++i) {
            tp.put_task(
                [&] () {
                    std::stringstream ss;
                    ss << "This tid is " << std::this_thread::get_id() << '\n';
                    std::cout << ss.str();
                    il::fiber::fiber::yield();
                    {
                        il::unique_lock<il::mutex> lock(num_guard);
                        ++num;
                    }
                }
            );
        }
    }

    std::cout << "Num after joinings is " << num << std::endl;

    return 0;
}