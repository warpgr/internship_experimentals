#pragma once

#include <concurrent/future.hpp>
#include <thread>
#include <functional>

namespace il {

template <typename FuncType>
class packaged_task;

template <typename RetType, typename... Args>
class packaged_task <RetType(Args...)> {
    public:
        packaged_task () noexcept {}
        template <typename Ts>
        packaged_task (Ts && func) {
            fun_ = std::forward<Ts>(func);
        }
        packaged_task (const packaged_task& other) = delete;
        packaged_task& operator = (packaged_task& other) = delete;
        packaged_task (packaged_task&& rhs) noexcept {swap(rhs);}
        packaged_task& operator = (packaged_task&& rhs ) {
             packaged_task(std::move(rhs)).swap(*this);
             return *this;
        }  
    public:
        void swap(packaged_task& other) {
            std::swap(prom_, other.prom_);
            fun_.swap(other.fun_);
        }
        template <typename... Ts>
        void operator()(Ts &&... ts) {
            prom_.set_value(fun_(std::forward<Ts>(ts)...)); 
        }
        
        future<RetType> get_future() 
        { 
            return prom_.get_future(); 
        }

        bool valid () const {
            return static_cast<bool>(fun_); 
        }

        void reset () {
            packaged_task(std::move(fun_)).swap(*this);
        } 

    private:
        promise<RetType>           prom_;
        std::function<RetType(Args...)> fun_;
};

// template<typename> class packaged_task;
// template< class R, class ...Args > 
// class packaged_task<R(Args...)>{

//     std::promise<R> p;
//     std::function<R(Args...)> fn; //use std::function to store function and arguments
//     public:
//         packaged_task() noexcept{}

//         //pass function and arguments
//         template <typename ...Ts>
//         explicit packaged_task(Ts &&... ts) : fn(std::forward<Ts>(ts)...) { }
        
//         //No copy
//         packaged_task(packaged_task&) = delete;
//         packaged_task& operator=(packaged_task&) = delete;

//         //move
//         packaged_task( packaged_task&& rhs ) noexcept{ swap(rhs); }
//         packaged_task& operator=(packaged_task&& __other)
//         {
//           packaged_task(std::move(__other)).swap(*this);
//           return *this;
//         }
        
//         //pass function and arguments
//         template <typename ...Ts>
//         void operator()(Ts &&... ts)
//         {
//             p.set_value(fn(std::forward<Ts>(ts)...)); 
//         }

//         std::future<R> get_future() 
//         { 
//             return p.get_future(); 
//         }

//         void swap(packaged_task& other){
//             p.swap(other.p);
//             fn.swap(other.fn);
//         }

//         bool valid() const{
//             return static_cast<bool>(fn); 
//         }
//        void reset()
//        {
//             packaged_task(std::move(fn)).swap(*this);
//        }
// };

} //namespace il