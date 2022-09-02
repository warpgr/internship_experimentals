#pragma once 


#include <concurrent/containers/mpmc_queue.hpp>
#include <concurrent/thread_pool.hpp>
#include <memory>

namespace il {

class function_executor {
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
        func(std::forward<Args>(args)...);
    }

    void execute_all() {
        while (true) {
            auto task = tasks_->pop();
            if (!task) { break; }
            execute(task);
        }
    }
};



thread_pool<function_executor>& default_thread_executor() {
    static thread_pool<function_executor> tp;
    return tp;
}

}
