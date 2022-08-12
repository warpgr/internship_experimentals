#include <concurrent/async.hpp>

#include <chrono>
#include <iostream>


int main() {
    auto fut = il::async(il::launch::deferred,
        [] () -> int {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            // std::cout << "setter thread after sleeping\n";
            std::cout << std::this_thread::get_id();
            return 9;
        }
    );
    std::thread th {
        [&fut] () {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // std::cout << "getter thread after sleeping\n";
            std::cout << std::this_thread::get_id();
            int res = fut.get();
            // std::cout << res << std::endl;
        }
    };
    th.join();

    return 0;
}