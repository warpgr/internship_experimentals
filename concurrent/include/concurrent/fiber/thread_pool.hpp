#pragma once
#include <concurrent/fiber/fiber.hpp>
#include <thread>
#include <functional>
#include <memory>

namespace il { namespace fiber {


template <typename TaskQueueTpye>
class fiber_executor {
    std::shared_ptr<TaskQueueTpye<std::function<void()>>>  tasks_;
public:
    template <typename FuncType, typename... Args>
    void submit_task(FuncType&& func, Args&&... args) {
        tasks_.push(
            [&] () {
                func(std::forward<Args>(args)...);
            });
    }
    template <typename FuncType, typename... Args>
    void execute(FuncType&& func, Args.. args) {
        auto packed_function = [&] () {
            func(std::forward<Args>(args)...);
        };
        fiber::create(packed_function, "fiber_from_executor");
    }

    void execute_all() {
        while (true) {
            auto task = tasks_.pop();
            if (!task) { return; }
            execute(task);
        }
    }
}

template <typename task_type, typename executor_type>
class thread_pool {
    executor_type<task_type>  executor_;
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
    void put_task(task_type&& task) {
        executor_.submit_task(task);
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;
private:
    void join() {
        for (int i = 0; i < workers_.size(); ++i) {
            submit_empty_task();
        }
        for (auto& worker : workers_) {
            if (worker.joinable()) { worker.join(); }
        }
    }
    void submit_empty_task() {
        executor_.submit_task({});
    }
};

// template<typename task_type>
// thread_pool<task_type>& default_tp() {
//     static thread_pool<task_type> tp;
//     return tp;
// }

}};