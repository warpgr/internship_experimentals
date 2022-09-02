#pragma once

#include <vector>
#include <thread>

namespace il {

template <typename executor_type>
class thread_pool {
    executor_type             executor_;
    std::vector<std::thread>  workers_;
public:
    thread_pool(size_t thread_count = 4) {
        for (int i = 0; i < thread_count; ++i) {
            workers_.emplace_back(std::thread(
                [this] () {
                    executor_.execute_all();
                }
            ));
        }
    }
    ~thread_pool() {
        join();
    }
    template <typename Func, typename... Args>
    void put_task(Func&& func, Args&&... args) {
        executor_.submit_task(std::forward<Func>(func), std::forward<Args>(args)...);
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;
private:
    void join() {
        for (int i = 0; i < workers_.size(); ++i) {
            executor_.submit_empty_task();
        }
        for (auto& worker : workers_) {
            if (worker.joinable()) { worker.join(); }
        }
    }
};

class function_executor;

template <typename executor_type = function_executor>
thread_pool<executor_type>& default_tp() {
    static thread_pool<executor_type> tp;
    return tp;
}

} // namespace il
