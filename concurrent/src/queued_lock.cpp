#include <concurrent/queued_lock.hpp>

namespace il {

void queed_lock::blocked_threads_queue::push_back(std::thread::id&& tid) {
    lock_guard<mutex> lock(threads_queue_guard_);
    threads_queue_.push(tid);
}
bool queed_lock::blocked_threads_queue::empty() {
    lock_guard<mutex> lock(threads_queue_guard_);
    return threads_queue_.empty();
}
std::thread::id queed_lock::blocked_threads_queue::front() {
    lock_guard<mutex> lock(threads_queue_guard_);
    return std::thread::id(threads_queue_.front());
}
void queed_lock::blocked_threads_queue::pop_front() {
    lock_guard<mutex> lock(threads_queue_guard_);
    threads_queue_.pop();
}

std::atomic<int> ticket_lock::ticket_ = { 0 };
std::atomic<int> ticket_lock::current_pos_ = { 0 };


}