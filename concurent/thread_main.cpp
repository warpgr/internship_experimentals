
#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <atomic>


// std::this_thread::get_id()
// std::this_thread::sleep_for(/*duratin*/);;
// std::this_thread::sleep_until(/*time_point*/);
// std::this_thread::yield();


class mutex {
    std::atomic<bool> captured_ = { false };
public:
    void lock() {
        while (captured_.exchange(true));
    }
    void unlock() {
        captured_ = false;
    }
};

int a = 0;
mutex m;

void foo() {
    // std::cout << "foo(): Before capturing lock" << std::endl; 
    m.lock();
    // std::cout << "foo(): After capturing lock" << std::endl; 
    a++;
    m.unlock();
}

int main(int argc, char** argv) {
    for (int i = 0;; ++i) {
        std::thread th_1{
        [&] () {
            // std::cout << "th_1::lambda(): Before capturing lock" << std::endl; 
            m.lock();
            // std::cout << "th_1::lambda(): After capturing lock" << std::endl; 
            --a;
            m.unlock();
        }
        };
        std::thread th_2 {foo};
        th_1.join();
        th_2.join();
        if (a != 0) {
            std::cout << "Crushed : a is " << a << " iter " << i << std::endl;
            exit(1); 
        }
    }
    
    // std::thread th_3{
    //     [&] () {
    //         std::cout << "th_3::lambda(): Before capturing lock" << std::endl; 
    //         m.lock();
    //         std::cout << "th_3::lambda(): After capturing lock" << std::endl; 
    //         --a;
    //         m.unlock();
    //     }
    // };
    // th_3.join();

    return 0;
}