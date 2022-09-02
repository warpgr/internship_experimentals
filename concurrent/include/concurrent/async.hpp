#pragma once
#include <thread>
#include <memory>
#include <concepts>

#include <concurrent/thread_pool.hpp>
#include <concurrent/future.hpp>
#include <concurrent/condition_variable.hpp>
#include <concurrent/executor.hpp>

namespace il {

template <typename FuncType, typename... Args>
auto async( const launch& launch_type, FuncType&& func, Args&&... args) 
    -> il::future<decltype(func(std::forward<Args>(args)...))> {
    auto prom = std::make_shared<il::promise<decltype(func(std::forward<Args>(args)...))>>();
    switch (launch_type) {
        case launch::deferred : {
            prom->set_callback(
                [&, task_promise = prom] {
                    task_promise->set_value(func(std::forward<Args>(args)...));
                }
            );
            break;
        }
        case launch::asynchronious :
        default : {
            auto& tp = default_thread_executor();
            tp.put_task(
                [&, task_promise = prom] () mutable {
                    auto res = func(std::forward<Args>(args)...);
                    task_promise->set_value(res);
                }
            );
            break;
        }
    }

    return prom->get_future();
}


} // namespace il


#define await_(fut) fut.get()

#define lazy_(func)  il::async(il::launch::deferred,        func)
#define async_(func) il::async(il::launch::asynchronious,   func)

