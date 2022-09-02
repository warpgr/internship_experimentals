#include <concurrent/fiber/future.hpp>
#include <concurrent/fiber/fiber.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>


void test_1() {
    il::fiber::promise<int> p;
    il::fiber::future<int> f = p.get_future();
    il::fiber::fiber::create(
        [&p] () {
            std::cout << "lambda(): Before seting value 1" << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
            p.set_value(4);
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
            std::cout << "lambda(): After seting value 1" << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));

            std::cout << "lambda(): Before seting value 2" << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
            p.set_value(5);
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
            std::cout << "lambda(): After seting value 2" << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
        }, "Sender"
    );

    il::fiber::fiber::create(
        [&f] () {
            f.wait_for(std::chrono::seconds(1));
            std::cout << "After 1 sec waiting " << std::endl;
            std::cout << f.get() << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));

            f.wait_for(std::chrono::seconds(1));
            std::cout << "After 1 sec waiting " << std::endl;
            std::cout << f.get() << std::endl;
            il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
        }, "Receiver"
    );
}


void test_2() {
    il::fiber::promise<int> generator_promise;
    il::fiber::future<int> generator_future = generator_promise.get_future();
    const int count = 10;
    std::vector<int> elements;
    il::fiber::fiber::create(
        [&generator_promise, &count] () {
            for (int i = 0; i < count; ++i) {
                generator_promise.set_value(i);
            }
        }, "generator Sender"
    );
    il::fiber::fiber::create(
        [&generator_future, &count, &elements] () {
            for (int i = 0; i < count; ++i) {
                elements.push_back(generator_future.get());
            }
        }, "Generator Receiver"
    );
    il::fiber::fiber::wait_for_all();
    std::cout << "After joining threads\n";
    assert (elements.size() == count);
    bool not_broken = true;
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (elements[i] >= elements[j] ) {
                not_broken = false;
            }
        }
    }
    assert(not_broken);
    for (auto& element : elements) {
        std::cout << element << " " << std::flush;
    }
    std::cout << "\n";
}


int main() {
    std::cout << "----------------Starting test_1()-----------------" <<std::endl;
    il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
    test_1();
    il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
    std::cout << "-------------End o test_1()------------------------------" << std::endl;
    il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
    std::cout << "----------------Starting test_2()-----------------" <<std::endl;
    test_2();
    il::fiber::fiber::this_fiber_sleep_for(std::chrono::seconds(3));
    std::cout << "-------------End of test_2()------------------------------" << std::endl;

    return 0;
}