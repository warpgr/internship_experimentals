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
    void put_task(TaskType&& task) {
        tasks_.push(std::move(task));
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;
private:
    void worker_routine() {
        while (true) {
            TaskType task = tasks_.pop();
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
} // namespace il
