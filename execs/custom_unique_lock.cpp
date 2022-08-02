#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <concurrent/unique_lock.hpp>

il::mutex guard;
int num = 0;

void print_and_sleep() {
    std::stringstream sstrem;
    sstrem << "do_something_unique_3(): My id : " << std::this_thread::get_id() << '\n';
    std::cout << sstrem.str() << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

il::unique_lock<il::mutex> do_something_unique_3() {
    il::unique_lock<il::mutex> lock(guard);
    ++num;
    // print_and_sleep();
    return std::move(lock);
}

il::unique_lock<il::mutex> do_something_unique_2() {
    /// doinng somethig
    // code unimportant state for locking mutex
    //  code unimportant state for locking mutex
    return std::move(do_something_unique_3());
}

il::unique_lock<il::mutex> do_something_unique_1() {
    // doing something
    ///code unimportant state for locking mutex
    ///code unimportant state for locking mutex
    return std::move(do_something_unique_2());
}

void do_something_unique_13(il::unique_lock<il::mutex>& lock) {
    lock.lock();
    ++num;
    // print_and_sleep();
}

void do_something_unique_12(il::unique_lock<il::mutex>& lock) {
    do_something_unique_13(lock);
}

void do_something_unique_11(il::unique_lock<il::mutex>& lock) {
    do_something_unique_12(lock);
}

void do_something_unique() {
    // il::unique_lock<il::mutex> lock(guard);
    auto&& something = do_something_unique_1();
    // std::stringstream sstrem;
    // sstrem << "do_something_unique(): My id : " << std::this_thread::get_id() << '\n';
    // std::cout << sstrem.str() << std::flush;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    ++num;
}

int main() {
    
    const int thread_count = 10;
    for (int j = 0;;++j) {
        std::vector<std::thread> threads;
        for (int i = 0; i < thread_count; ++i) {
            threads.push_back(std::thread {do_something_unique});
        }

        for (auto& thread : threads) {
            thread.join();
        }
        if ( num != 20 ) {
            std::cout << "crushed " << std::endl;
        }
        num = 0;
    }
    



    return 0;
}