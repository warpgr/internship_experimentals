#include <deque>
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
        bool erase(T element);
        bool contains(T element);
    private:
        std::deque<T>                  elements_;
        mutable il::mutex              elements_guard_;
        il::condition_variable<il::unique_lock<il::mutex>>
                                       is_not_empty_;
    private:
        T take();
};


template <typename T>
bool mpmc_queue<T>::empty() const {
    il::unique_lock<il::mutex> lock(elements_guard_);
    return elements_.empty();
}

template <typename T>
void mpmc_queue<T>::push(const T&& data) {
    il::unique_lock<il::mutex> lock(elements_guard_);
    elements_.push_back(std::move(data));
    is_not_empty_.notify_one();
}

template <typename T>
boost::optional<T> mpmc_queue<T>::try_pop() {
    il::unique_lock<il::mutex> lock(elements_guard_);
    if (elements_.empty()) {
        return boost::optional<T>();
    }
    return take();
}

template <typename T>
T mpmc_queue<T>::pop() {
    il::unique_lock<il::mutex> lock(elements_guard_);
    while (elements_.empty()) {
        is_not_empty_.wait(lock);
    }
    return take();
}

template <typename T>
T mpmc_queue<T>::take() {
    assert (!elements_.empty());
    T item =  std::move(elements_.front());
    elements_.pop_front();
    return item;
}

template <typename T> 
bool mpmc_queue<T>::erase(T element) {
    il::unique_lock<il::mutex> lock(elements_guard_);
    auto iter = elements_.erase(
        std::remove_if(elements_.begin(), elements_.end(), 
            [element] (T el) { return el == element; }));
    return (elements_.end() != iter) ? true : false;
}

template <typename T>
bool mpmc_queue<T>::contains(T element) {
    il::unique_lock<il::mutex> lock(elements_guard_);
    return (elements_.end() != std::find(elements_.begin(), elements_.end(), element) == elements_.end())
        ? true : false;
}

} // namespace il