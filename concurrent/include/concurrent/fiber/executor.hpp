#pragma once
#include <concurrent/fiber/fiber.hpp>
#include <thread>
#include <functional>
#include <memory>
#include <concurrent/containers/mpmc_queue.hpp>
#include <iostream>


namespace il { namespace fiber {

class fiber_executor {
    std::shared_ptr<mpmc_queue<std::function<void()>>>     tasks_;
public:
    fiber_executor() {
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
        auto packed_function = [&] () {
            func(std::forward<Args>(args)...);
        };
        auto current_fiber = fiber::create(packed_function, "fiber_from_executor");
        fiber::yield_to(current_fiber);
    }

    void execute_all() {
        while (true) {
            auto task = tasks_->pop();
            if (!task) { break; }
            execute(task);
        }
        while (fiber::get_scheduler()->is_not_empty_queue()) {
            fiber::yield();
        }
    }
};

}};
