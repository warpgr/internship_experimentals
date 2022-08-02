#include <concurrent/queued_lock.hpp>
#include <vector>



int main(int argc, char** argv) {


    int num = 0; // guarded by mutex
    il::mutex num_guard;

    const int thread_count  = 10;
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.push_back(
            std::thread{
                [&] () {
                    il::ticket_lock lock(num_guard);
                    ++num;
                }
            }
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "num is " << num << std::endl;


    return 0;
}