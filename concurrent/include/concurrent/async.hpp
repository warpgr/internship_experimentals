#pragma once
#include <thread>
#include <memory>

#include <concurrent/thread_pool.hpp>
#include <concurrent/future.hpp>

namespace il {

template <std::invocable Func>
auto async(Func func) -> il::future<decltype(func())> {
    auto prom = std::make_shared<il::promise<decltype(func())>>();

    auto& tp = default_tp<std::function<void()>>();
    tp.put_task(
        [task_promise = prom, &func] () mutable {
            decltype(auto) res = func();
            task_promise->set_value(res);
        }
    );
    return prom->get_future();
}

} // namespace il
