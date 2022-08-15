#include <concurrent/async.hpp>

#include <chrono>
#include <iostream>
int foo(int value) {
    std::cout << "Passed " << value << " sdad " << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return value + 1;
}

int main() {
    std::cout << "Main tid " << std::this_thread::get_id() << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    auto res_fut = il::async(il::launch::asynchronious,
        [] () -> int {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // std::cout << "setter thread after sleeping\n";
            std::cout << std::this_thread::get_id() << std::endl;
            return 1;
        }
    )
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo);

    auto res_fut_2 = il::async(il::launch::asynchronious,
        [] () -> int {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // std::cout << "setter thread after sleeping\n";
            std::cout << std::this_thread::get_id() << std::endl;
            return 10;
        }
    )
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo);

    std::cout << "Main" << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Final res 1 " << res_fut.get() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Final res 2 " << res_fut_2.get() << std::endl;
    // .then([] (int val) -> int { return 2; });
    return 0;
}