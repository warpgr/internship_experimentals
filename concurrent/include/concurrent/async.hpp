#pragma once
#include <thread>
#include <memory>
#include <concepts>

#include <concurrent/thread_pool.hpp>
#include <concurrent/future.hpp>

namespace il {

template <std::invocable Func>
auto async( const launch& launch_type, Func func) 
    -> il::future<decltype(func())> {
    auto prom = std::make_shared<il::promise<decltype(func())>>();
    switch (launch_type) {
        case launch::deferred: {
            prom->set_callback(
                [&, task_promise = prom] {
                    task_promise->set_value(func());
                }
            );
            break;
        }
        case launch::asynchronious:
        default: {
            auto& tp = default_tp<std::function<void()>>();
            tp.put_task(
                [task_promise = prom, &func] () mutable {
                    decltype(func()) res = func();
                    task_promise->set_value(res);
                }
            );
            break;
        }
    }
    
    return prom->get_future();
}

#define await(fut) fut.get()

#define lazy_(func) il::async(il::launch::deferred, func)
#define async_(func) il::async(il::launch::asynchronious, func)

} // namespace il
