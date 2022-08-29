#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <concepts>

#include <concurrent/containers/mpmc_queue.hpp>

namespace il {


class function_executor {
    // std::shared_ptr<TaskQueueTpye<std::function<void()>>>  tasks_;
    std::shared_ptr<mpmc_queue<std::function<void()>>>     tasks_;
public:
    function_executor() {
        tasks_ = std::make_shared<mpmc_queue<std::function<void()>>>();
    }
    template <typename FuncType, typename... Args>
    void submit_task(FuncType&& func, Args&&... args) {
        tasks_->push(
            [&] () {
                func(std::forward<Args>(args)...);
            });
    }

    void submit_empty_task() {
        tasks_->push({});
    }
    template <typename FuncType, typename... Args>
    void execute(FuncType&& func, Args... args) {
        [&] () {
            func(std::forward<Args>(args)...);
        }();
    }

    void execute_all() {
        while (true) {
            auto task = tasks_->pop();
            if (!task) { break; }
            execute(task);
        }
    }
};

template <typename executor_type = function_executor>
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

template<typename executor_type = function_executor>
thread_pool<executor_type>& default_tp() {
    static thread_pool<executor_type> tp;
    return tp;
}
} // namespace il
