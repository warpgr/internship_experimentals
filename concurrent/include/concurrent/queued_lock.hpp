#pragma once
#include <queue>
#include <thread>
#include <concurrent/mutex.hpp>
#include <iostream>
#include <sstream>


namespace il {

template <Mutex mutex_type = il::mutex>
class queed_lock {
    struct blocked_threads_queue {
    private:
        std::queue<std::thread::id> threads_queue_;
        mutex_type                  threads_queue_guard_;
    public:
        blocked_threads_queue() = default;
        void push_back(std::thread::id&& tid);
        bool empty();
        
        std::thread::id front();
        void pop_front();
    }                                                threads_queue_;
    mutex_type&                                      m_;

    struct sstream_out {
        // TODO: Implement sstream with blocking for logging queue of threads.

    };
public:
    queed_lock(mutex_type& m)
        : m_(m) {
        threads_queue_.push_back(std::this_thread::get_id());
        std::cout << "Im in critical section and my id is "
                    << std::this_thread::get_id() << std::endl << std::flush;
        while (!threads_queue_.empty() && 
                threads_queue_.front() == std::this_thread::get_id()) {
            m_.lock();
            std::cout << "Im captured the lock and my id is "
                    << std::this_thread::get_id() << std::endl << std::flush;
            break;
        }
    }
    ~queed_lock() {
        threads_queue_.pop_front();
        m_.unlock();
        std::cout << "Im have released the lock and my id is "
                  << std::this_thread::get_id() << std::endl << std::flush;
    }
};


template <Mutex mutex_type = mutex>
class ticket_lock {
    static std::atomic<int> ticket_;
    static std::atomic<int> current_pos_;
    mutex_type&             m_;
public:
    ticket_lock() = delete;
    ticket_lock(mutex_type& m)
        : m_(m) {
            int ticket = ticket_.fetch_add(1);
            while (ticket != current_pos_);
            m_.lock();
    }
    ~ticket_lock() {
        current_pos_.fetch_add(1);
        m_.unlock();
    }
    ticket_lock(const ticket_lock& other) = delete;
    ticket_lock& operator=(const ticket_lock& other) = delete;
};

template <Mutex mutex_type>
void queed_lock<mutex_type>::blocked_threads_queue::push_back(std::thread::id&& tid) {
    lock_guard<mutex> lock(threads_queue_guard_);
    threads_queue_.push(tid);
}

template <Mutex mutex_type>
bool queed_lock<mutex_type>::blocked_threads_queue::empty() {
    lock_guard<mutex> lock(threads_queue_guard_);
    return threads_queue_.empty();
}

template <Mutex mutex_type>
std::thread::id queed_lock<mutex_type>::blocked_threads_queue::front() {
    lock_guard<mutex> lock(threads_queue_guard_);
    return std::thread::id(threads_queue_.front());
}

template <Mutex mutex_type>
void queed_lock<mutex_type>::blocked_threads_queue::pop_front() {
    lock_guard<mutex> lock(threads_queue_guard_);
    threads_queue_.pop();
}

template <Mutex mutex_type>
std::atomic<int> ticket_lock<mutex_type>::ticket_ = { 0 };

template <Mutex mutex_type>
std::atomic<int> ticket_lock<mutex_type>::current_pos_ = { 0 };


} // namespace li
