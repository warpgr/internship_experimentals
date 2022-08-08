#include <queue>
#include <boost/optional.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <concurrent/condition_variable.hpp>
#include <condition_variable>
#include <sstream>

#include <iostream>

namespace il {

template <typename T>
class mpmc_queue {
    public:
        mpmc_queue () = default;
        mpmc_queue (const mpmc_queue &) = delete;
        mpmc_queue (mpmc_queue &&) = delete;
        mpmc_queue &operator = (const mpmc_queue &) = delete;
        mpmc_queue &operator = (mpmc_queue &&) = delete;
    public:
        bool empty() const;
        void push(const T&& data);
        boost::optional<T> try_pop();
        T pop();
    private:
        std::queue<T>                  elements_;
        mutable std::mutex             elements_guard_;
        std::condition_variable
                                       is_not_empty_;
    private:
        T take();
};


template <typename T>
bool mpmc_queue<T>::empty() const {
    std::unique_lock<std::mutex> lock(elements_guard_);
    return elements_.empty();
}

template <typename T>
void mpmc_queue<T>::push(const T&& data) {
    std::unique_lock<std::mutex> lock(elements_guard_);
    elements_.push(std::move(data));
    is_not_empty_.notify_one();
}

template <typename T>
boost::optional<T> mpmc_queue<T>::try_pop() {
    std::unique_lock<std::mutex> lock(elements_guard_);
    if (elements_.empty()) {
        return boost::optional<T>();
    }
    return take();
}

template <typename T>
T mpmc_queue<T>::pop() {
    std::unique_lock<std::mutex> lock(elements_guard_);
    while (elements_.empty()) {
        is_not_empty_.wait(lock);
    }
    return take();
}

template <typename T>
T mpmc_queue<T>::take() {
    assert (!elements_.empty());
    T item =  std::move(elements_.front());
    elements_.pop();
    return item;
}

} // namespace il