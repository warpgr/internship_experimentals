#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

int main() {
    std::vector<std::thread> threads;
    int shared_num = 0;
    std::mutex num_guard;
    const int thread_count = 10;
    
    for (int i = 0; i < thread_count; ++i) {
        if ( 0 == i % 2) {
            threads.push_back(
                std::thread(
                    [&num_guard, &shared_num, num = i] () {
                        try {
                             std::lock_guard<std::mutex> guard(num_guard);
                            throw 1;
                            shared_num += num;
                        }
                        catch (int&) {
                            // ... 
                        }
                       
                    }
                )
            );
        }
        else {
             threads.push_back(
                std::thread(
                    [&num_guard, &shared_num, num = i] () {
                        std::lock_guard<std::mutex> guard(num_guard);
                        shared_num += num;
                    }
                )
            );
        }
       
    }
    for (int i = 0; i < thread_count; ++i) {
        if (threads.at(i).joinable()) {
            threads.at(i).join();
        }
    }

    std::cout << "After work num is " << shared_num << std::endl;
    return 0;
}