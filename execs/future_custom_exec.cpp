#include <concurrent/future.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>


void test_1() {
    il::promise<int> p;
    il::future<int> f = p.get_future();
    std::thread th {
        [&p] () {
            std::cout << "lambda(): Before seting value 1" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            p.set_value(4);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "lambda(): After seting value 1" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "lambda(): Before seting value 2" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            p.set_value(5);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "lambda(): After seting value 2" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    std::thread future_thread {
        [&f] () {
            f.wait_for(std::chrono::seconds(1));
            std::cout << "After 1 sec waiting " << std::endl;
            std::cout << f.get() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));

            f.wait_for(std::chrono::seconds(1));
            std::cout << "After 1 sec waiting " << std::endl;
            std::cout << f.get() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };
    
    th.join();
    future_thread.join();
}


void test_2() {
    il::promise<int> generator_promise;
    il::future<int> generator_future = generator_promise.get_future();
    const int count = 100000;
    std::vector<int> elements;
    std::thread sender_thread {
        [&generator_promise, &count] () {
            for (int i = 0; i < count; ++i) {
                generator_promise.set_value(i);
            }
        }
    };

    std::thread receiver_thread {
        [&generator_future, &count, &elements] () {
            for (int i = 0; i < count; ++i) {
                elements.push_back(generator_future.get());
            }
        }
    };

    std::cout << "Before joining threads\n";
    sender_thread.join();
    receiver_thread.join();
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    test_1();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "-------------End o test_1()------------------------------" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "----------------Starting test_2()-----------------" <<std::endl;
    test_2();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "-------------End o test_2()------------------------------" << std::endl;


    return 0;
}