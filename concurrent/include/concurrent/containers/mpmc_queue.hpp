#include <queue>
#include <boost/optional.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <concurrent/condition_variable.hpp>

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
        mutable mutex                  elements_guard_;
        il::condition_variable<unique_lock<mutex>>
                                       is_not_empty_;
};

template <typename T> 
bool mpmc_queue<T>::empty() const {
    unique_lock<mutex> lock(elements_guard_);
    return elements_.empty();
}

template <typename T>
void mpmc_queue<T>::push(const T&& data) {
    unique_lock<mutex> lock(elements_guard_);
    elements_.push(std::move(data));
    is_not_empty_.notify_all();
}

template <typename T>
boost::optional<T> mpmc_queue<T>::try_pop() {
    unique_lock<mutex> lock(elements_guard_);
    if (elements_.empty()) {
        return boost::optional<T>();
    }
    T data(std::move(elements_.front()));
    elements_.pop();
    return data;
}

template <typename T>
T mpmc_queue<T>::pop() {
    unique_lock<mutex> lock(elements_guard_);
    while (empty()) {
        is_not_empty_.wait(lock);
    }
    T element(std::move(elements_.front()));
    elements_.pop();
    return element;
}

} // namespace il