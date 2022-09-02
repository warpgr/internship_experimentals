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
        promise<RetType>                prom_;
        std::function<RetType(Args...)> fun_;
};

} //namespace il
