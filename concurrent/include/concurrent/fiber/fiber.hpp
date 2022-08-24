#pragma once
#include <memory>
#include <functional>
// #include <concurrent/fiber/scheduler.hpp>
#include <fiber/fiber_impl.hpp>

namespace il { namespace fiber {

class fiber {
public:
//     static fiber& get_main_fiber();
//     static fiber* get_active_fiber();
//     static std::unique_ptr<fiber> create(const std::function<void()>& function, const std::string& name = "fiber");
//     static void yield_to(fiber& f);
//     static void yield();
//     static void set_scheduler(std::unique_ptr<scheduler> new_scheduler);

// public:
//     ~fiber();

//     const std::string& name() const {
//         return fiber_name_;
//     }

//     bool is_active() const {
//         return data_->active_fiber_ == this;
//     }

//     bool is_finished() const {
//         return is_finished_;
//     }

//     bool is_main_fiber() const {
//         return is_main_fiber_;
//     }

// private:
//     static void fiber_main(fiber* fib);
//     fiber(const std::function<void()>& func, const std::string& name, bool is_main_fiber);
//     fiber(const fiber& other) = delete;
//     fiber(fiber&& other) = delete;
//     fiber& operator=(const fiber& other) = delete;
//     fiber& operator=(fiber&& other) = delete;

//     bool is_valid() const {
//         return impl_.is_valid();
//     }

//     void finish();

//     struct thread_local_data {
//         thread_local_data(std::unique_ptr<scheduler> initial_shceduler)
//             : active_fiber_(nullptr), scheduler_(std::move(initial_shceduler)) { }

//         fiber*                     active_fiber_;
//         std::unique_ptr<scheduler> scheduler_;
//         std::vector<fiber*>        fibers_;
//     };

private:

    // static thread_local std::shared_ptr<thread_local_data> thread_local_d;
    // std::function<void()>              function_;
    // std::string                        fiber_name_;
    // const bool                         is_main_fiber_;
    // bool                               is_finished_;
    // fiber_impl                         impl_;
    // std::shared_ptr<thread_local_data> data_;
};

}}
