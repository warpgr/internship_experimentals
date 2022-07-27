#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <concurrent/mutex.hpp>
#include <iostream>
#include <sstream>

namespace il {

class queed_lock {
    struct blocked_threads_queue {
    private:
        std::queue<std::thread::id> threads_queue_;
        mutex                       threads_queue_guard_;
    public:
        blocked_threads_queue() = default;
        void push_back(std::thread::id&& tid);
        bool empty();
        std::thread::id front();
        void pop_front();
    }                                                threads_queue_;
    mutex&                                           m_;

    struct sstream_out {
        // TODO: Implement sstream with blocking for logging queue of threads
    };
public:
    queed_lock(mutex& m)
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


class ticket_lock {

public:
    // TODO: Ticket lock will atomicly give threads incrementable value,
    // which will be their ticket number for capture lock.
    // Ticket must get at enerance of critical section.

};


}