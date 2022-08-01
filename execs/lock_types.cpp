#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <shared_mutex>

// std::mutex guard;
std::shared_mutex shared_guard;
int num = 0;

void print_and_sleep() {
    std::stringstream sstrem;
    sstrem << "do_something_unique_3(): My id : " << std::this_thread::get_id() << '\n';
    std::cout << sstrem.str() << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

std::unique_lock<std::shared_mutex> do_something_unique_3() {
    std::unique_lock<std::shared_mutex> lock(shared_guard);
    ++num;
    print_and_sleep();
    return std::move(lock);
}

std::unique_lock<std::shared_mutex> do_something_unique_2() {
    /// doinng somethig
    // code unimportant state for locking mutex
    //  code unimportant state for locking mutex
    return std::move(do_something_unique_3());
}

std::unique_lock<std::shared_mutex> do_something_unique_1() {
    // doing something
    ///code unimportant state for locking mutex
    ///code unimportant state for locking mutex
    return std::move(do_something_unique_2());
}

void do_something_unique_13(std::unique_lock<std::shared_mutex>& lock) {
    lock.lock();
    ++num;
    print_and_sleep();
}

void do_something_unique_12(std::unique_lock<std::shared_mutex>& lock) {
    do_something_unique_13(lock);
}

void do_something_unique_11(std::unique_lock<std::shared_mutex>& lock) {
    do_something_unique_12(lock);
}

void do_something_unique() {
    std::unique_lock<std::shared_mutex> lock(shared_guard, std::defer_lock);
    auto&& something = do_something_unique_1();
    std::stringstream sstrem;
    sstrem << "do_something_unique(): My id : " << std::this_thread::get_id() << '\n';
    std::cout << sstrem.str() << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ++num;
}

void do_something_shared() {
    std::shared_lock<std::shared_mutex> lock(shared_guard);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::stringstream sstrem;
    sstrem << "My id : " << std::this_thread::get_id() << " Num is " << num << '\n';
    std::cout << sstrem.str();
}


int main() {
    std::vector<std::thread> threads;
    const int thread_count = 10;
    for (int i = 0; i < thread_count; ++i) {
        // if (i > 7) {
            threads.push_back(std::thread {do_something_unique});
            // continue;
        // }
        // threads.push_back(std::thread {do_something_shared});
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "num is " << num << std::endl;



    return 0;
}