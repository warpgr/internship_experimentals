#include <concurrent/thread_pool.hpp>
#include <functional>
#include <sstream>
#include <iostream>
#include <memory>



int main() {

    size_t shared_num = 0;
    il::mutex num_guard;
    {
        // std::shared_ptr<il::thread_pool<std::function<void()>>> tp_ptr = std::make_shared<il::thread_pool<std::function<void()>>>(10);
        auto tp_ptr = &il::default_tp();
        std::cout << "main(): after thread_pool creation" << std::endl;
        for (int i = 0; i < 100; ++i) {
            std::stringstream sstrea;
            sstrea <<  "iter " << i << '\n';
            std::cout << sstrea.str();
            tp_ptr->put_task(std::move(
                [&] () -> void { // task from main
                    il::lock_guard<il::mutex> lock(num_guard);
                    ++shared_num;
                    std::stringstream sstream;
                    sstream << "Current increment thread ID is " << std::this_thread::get_id() << '\n';
                    std::cout << sstream.str();
                    // auto another_task =  [&tp_ptr, &num_guard, &shared_num] () -> void { // task from thread pool worker
                    //     il::lock_guard<il::mutex> lock(num_guard);
                    //     --shared_num;
                    //     std::stringstream sstream;
                    //     sstream << "Current decrement thread ID is " << std::this_thread::get_id() << '\n';
                    //     std::cout << sstream.str();
                    // };
                    // tp_ptr->put_task( another_task );
                })
            );
        }
        // std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "main(): after puting tasks" << std::endl;
    }

    std::cout << "shared_num is " << shared_num << std::endl;

    return 0;
}