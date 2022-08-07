#include <concurrent/queued_lock.hpp>

namespace il {

template <Mutex mutex_type>
void queed_lock<mutex_type>::blocked_threads_queue::push_back(std::thread::id&& tid) {
    lock_guard<mutex_type> lock(threads_queue_guard_);
    threads_queue_.push(tid);
}

template <Mutex mutex_type>
bool queed_lock<mutex_type>::blocked_threads_queue::empty() {
    lock_guard<mutex_type> lock(threads_queue_guard_);
    return threads_queue_.empty();
}

template <Mutex mutex_type>
std::thread::id queed_lock<mutex_type>::blocked_threads_queue::front() {
    lock_guard<mutex_type> lock(threads_queue_guard_);
    return std::thread::id(threads_queue_.front());
}

template <Mutex mutex_type>
void queed_lock<mutex_type>::blocked_threads_queue::pop_front() {
    lock_guard<mutex_type> lock(threads_queue_guard_);
    threads_queue_.pop();
}

}
