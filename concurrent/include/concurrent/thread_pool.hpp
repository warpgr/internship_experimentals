#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <concepts>

#include <concurrent/containers/mpmc_queue.hpp>

namespace il {

template <std::invocable task_type>
class thread_pool {
    mpmc_queue<task_type>     tasks_;
    std::vector<std::thread> workers_;
public:
    thread_pool(size_t thread_count = 4) {
        for (int i = 0; i < thread_count; ++i) {
            workers_.emplace_back(std::thread(
                [this] () {
                    worker_routine();
                }
            ));
        }
    }
    ~thread_pool() {
        while (!tasks_.empty());// TODO:
        join();
    }
    void put_task(task_type&& task) {
        tasks_.push(std::move(task));
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;
private:
    void worker_routine() {
        while (true) {
            task_type task = tasks_.pop();
            if (!task) {
                break;
            }
            task();
        }
    }
    void join() {
        for (int i = 0; i < workers_.size(); ++i) {
            tasks_.push({});
        }
        for (auto& worker : workers_) {
            if (worker.joinable()) { worker.join(); }
        }
    }
};

template<std::invocable task_type>
thread_pool<task_type>& default_tp() {
    static thread_pool<task_type> tp;
    return tp;
}
} // namespace il
