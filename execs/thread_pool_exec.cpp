#include <concurrent/thread_pool.hpp>
#include <functional>
#include <sstream>
#include <iostream>



int main() {

    size_t shared_num = 0;
    il::mutex num_guard;
    {
        il::thread_pool<std::function<void()>> tp;
        std::cout << "main(): after thread_pool creation" << std::endl;
        std::thread{
            [&] () {
                for (int i = 0; i < 100; ++i) {
                    std::cout << "iter " << i << std::endl << std::flush;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    tp.put_task(
                        [&shared_num, &num_guard] () {
                            il::lock_guard<il::mutex> lock(num_guard);
                            ++shared_num;
                            std::stringstream sstream;
                            sstream << "Current thread ID is " << std::this_thread::get_id() << '\n';
                            std::cout << sstream.str();
                        }
                    );
                }
            }
        }.detach();
        std::cout << "main(): after puting tasks" << std::endl;
    }

    std::cout << "shared_num is " << shared_num << std::endl;

    return 0;
}