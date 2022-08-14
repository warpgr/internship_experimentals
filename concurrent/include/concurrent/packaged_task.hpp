#pragma once

#include <future>
#include <thread>

template <typename func>
class pack_task;

template <typename T, typename... Args>
class pack_task<T(Args...)> {
    public:
        pack_task () noexcept {}
        template <typename... Ts>
        pack_task (Ts &&... ts): fun_ (std::forward<Ts>(ts)...) {} 
        pack_task (const pack_task& other) = delete;
        pack_task& operator = (pack_task& other) = delete;
        pack_task (pack_task&& rhs) noexcept {swap(rhs);}
        pack_task& operator = (pack_task&& rhs ) {
             pack_task(std::move(rhs)).swap(*this);
             return *this;
        }  
    public:
        void swap(pack_task& other){
            prom_.swap(other.prom_);
            fun_.swap(other.fun_);
        }

        template <typename ...Ts>
        void operator()(Ts &&... ts) {
            prom_.set_value(fun_(std::forward<Ts>(ts)...)); 
        }
        
        std::future<T> get_future() 
        { 
            return prom_.get_future(); 
        }

        bool valid () const {

        }

        void reset () {
            
        } 

    private:
        std::promise<T> prom_;
        std::function<T(Args...)> fun_;
};