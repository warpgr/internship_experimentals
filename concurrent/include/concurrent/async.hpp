#pragma once
#include <concurrent/future.hpp>
#include <thread>
#include <memory>
#include <concurrent/thread_pool.hpp>

namespace il {

template <typename Func, typename... Args>
auto async(Func func, Args... args) 
    -> il::future<decltype(func(std::forward<Args>(args)...))> {
    auto prom = std::make_shared<il::promise<decltype(func(std::forward<Args>(args)...))>>();

    auto& tp = default_tp<std::function<void()>>();
    tp.put_task(
        [task_promise = prom, &func, &args...] () mutable {
            decltype(func(std::forward<Args>(args)...)) res = func(std::forward<Args>(args)...);
            task_promise->set_value(res);
        }
    );
    return prom->get_future();
}


}