#include <concurrent/queued_lock.hpp>
#include <vector>
#include <iostream>



int main() {

    std::vector<std::thread> threads;
    int shared_num = 0;
    il::mutex num_guard;
    const int thread_count = 10;
    
    for (int i = 0; i < thread_count; ++i) {
        if ( 0 == i % 2) {
            threads.push_back(
                std::thread(
                    [&num_guard, &shared_num, num = i] () {
                        try {
                            il::queed_lock guard(num_guard);
                            throw 1;
                            shared_num += num;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                        il::queed_lock guard(num_guard);
                        shared_num += num;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
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


    return 0;
}