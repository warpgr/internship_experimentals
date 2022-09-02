
#include <concurrent/fiber/async.hpp>
#include <thread>
#include <chrono>
#include <iostream>

int foo() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "From foo() tid is " << std::this_thread::get_id() << std::endl;
    return 1;
}

int main () {

    auto future_value = il::fiber::async(il::launch::asynchronious, foo);
    int val = future_value.get();

    std::cout << val << std::endl;

    return 0;
}