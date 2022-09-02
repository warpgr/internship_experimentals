#pragma once

#include <concurrent/fiber/future.hpp>
#include <concurrent/fiber/executor.hpp>

namespace il { namespace fiber {

template <typename FuncType, typename... Args>
auto async( const launch& launch_type, FuncType&& func, Args&&... args) 
    -> future<decltype(func(std::forward<Args>(args)...))> {
    auto prom = std::make_shared<promise<decltype(func(std::forward<Args>(args)...))>>();
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
            auto& tp = default_fiber_executor();
            tp.put_task(
                [&, task_promise = prom] () mutable {
                    decltype(func()) res = func(std::forward<Args>(args)...);
                    task_promise->set_value(res);
                }
            );
            break;
        }
    }

    return prom->get_future();
}


#define await_(fut) fut.get()

#define lazy_ (func) il::async(il::launch::deferred,        func)
#define async_(func) il::async(il::launch::asynchronious,   func)

}}
