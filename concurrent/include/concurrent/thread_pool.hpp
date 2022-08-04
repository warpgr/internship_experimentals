#pragma once

#include <concurrent/containers/mpmc_queue.hpp>
#include <vector>
#include <thread>
#include <atomic>

namespace il {

template <typename TaskType>
class thread_pool {
    mpmc_queue<TaskType>     tasks_;
    std::vector<std::thread> workers_;
public:
    thread_pool(size_t thread_count = 4) {
        for (int i = 0; i < thread_count; ++i) {
            workers_.push_back(std::thread(&thread_pool::worker_routine, this));
        }
    }
    ~thread_pool() {
        join();
    }
    void put_task(TaskType task) {
        tasks_.push(std::move(task));
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;

private:
    void worker_routine() {
        while (true) {
            auto task = tasks_.pop();
            if (!task) { return; }
            task();
        }
    }
    void join() {
        for (int i = 0; i < workers_.size(); ++i) {
            tasks_.push(TaskType{});
        }
        for (auto& worker : workers_) {
            if (worker.joinable()){ worker.join(); }
        }
    }
};
} // namespace il
