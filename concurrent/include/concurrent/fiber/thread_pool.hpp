#pragma once
#include <concurrent/fiber/scheduler.hpp>
#include <thread>
#include <functional>
#include <memory>

namespace il { namespace fiber {



template <typename scheduller_type>
class fiber_and_thread {
    struct thread_local_data {
        std::shared_ptr<scheduller_type> scheduller_;
        fiber*                           active_fiber_;
    };
    static thread_local
    std::unique_ptr<thread_local_data> 
                                     thread_local_data_ = std::make_unique<thread_local_data>();
    std::thread                      thread_;
    
private:
    void worker_routine() {
        while (true) {
            fiber* next_fiber = thread_local_data_->scheduller_->next(active_fiber_);
            fiber::yield_to(*active_fiber_, *next_fiber);
        }
    }
public:
    fiber_and_thread(std::shared_ptr<scheduller_type> scheduller) {
        set_scheduller(scheduller);
    }

    void set_scheduler(std::shared_ptr<scheduller_type> new_scheduler) {
        thread_local_data_->scheduller_ = scheduller;
    }

public:
    static void yield();
};

template <typename scheduller_type>
void fiber_and_thread<scheduller_type>::yield() {
    assert(thread_local_data_ && thread_local_data_->active_fiber_ && thread_local_data_->scheduler_);

    fiber* next_fib = thread_local_data_->scheduler_->next();
    if (!next_fib) { return; }

    assert(next_fib->is_valid() && !next_fib->is_active() && !next_fib->is_finished());
    assert(next_fib != thread_local_data_->active_fiber_);

    fiber* current_fib = thread_local_data_->active_fiber_;
    thread_local_data_->active_fiber_ = next_fib;
    yield_to(*current_fib, *next_fib);
}



template <typename task_type, typename scheduller_type = round_robin_scheduler>
class thread_pool {
    scheduller_type           scheduller_;
    std::vector<std::thread>  workers_;
public:
    thread_pool(size_t thread_count = 4) {
        for (int i = 0; i < thread_count; ++i) {
            workers_.emplace_back(std::thread(
                [this] () {
                    worker_routine();
                }
            ));
        }
    }
    ~thread_pool() {
        while (!tasks_.empty());// TODO:
        join();
    }
    void put_task(task_type&& task) {
        tasks_.push(std::move(task));
    }
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool&& operator=(thread_pool&&) = delete;
private:
    
    void join() {
        // for (int i = 0; i < workers_.size(); ++i) {
        //     tasks_.push({});
        // }
        // for (auto& worker : workers_) {
        //     if (worker.joinable()) { worker.join(); }
        // }
    }
};

// template<typename task_type>
// thread_pool<task_type>& default_tp() {
//     static thread_pool<task_type> tp;
//     return tp;
// }

}};