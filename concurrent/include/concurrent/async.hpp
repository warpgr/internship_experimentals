#pragma once
#include <concurrent/future.hpp>
#include <thread>
#include <memory>
#include <concurrent/thread_pool.hpp>

namespace il {

enum launch {
    asynchronious = 1,
    deferred = 2
};

template <typename Func, typename... Args>
auto async( const launch& launch_type, Func func, Args... args) 
    -> il::future<decltype(func(std::forward<Args>(args)...))> {
    auto prom = std::make_shared<il::promise<decltype(func(std::forward<Args>(args)...))>>();
    switch (launch_type) {
        case launch::deferred: {
            prom->set_callback(
                [&, task_promise = prom] {
                    task_promise->set_value(func(std::forward<Args>(args)...));
                }
            );
            break;
        }
        case launch::asynchronious:
        default: {
            auto& tp = default_tp<std::function<void()>>();
            tp.put_task(
                [task_promise = prom, &func, &args...] () mutable {
                    decltype(func(std::forward<Args>(args)...)) res = func(std::forward<Args>(args)...);
                    task_promise->set_value(res);
                }
            );
            break;
        }
    }
    
    return prom->get_future();
}


}