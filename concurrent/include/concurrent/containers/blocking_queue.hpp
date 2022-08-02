#include <queue>
#include <mutex>
#include <optional>

namespace il {

template <typename T>
class block_queue {
    public:
        block_queue(size_t capacity): capacity_(capacity) {}
        block_queue (const block_queue &) = delete;
        block_queue (block_queue &&) = delete;
        block_queue &operator = (const block_queue &) = delete;
        block_queue &operator = (block_queue &&) = delete;
    public:
        bool empty() const;
        void push(const T&& data);
        bool try_push(const T&& data);
        std::optional<T> pop(T& data);
        bool try_pop(T& data);
        void print_queue() const;
    private:
        std::queue<T> qu_;
        std::mutex guard_;
        size_t capacity_;
};

template <typename T> 
bool block_queue<T>::empty() const {
    std::unique_lock<std::mutex> lm(guard_);
    return qu_.empty();
}

template <typename T>
void block_queue<T>::push(const T&& data) {
    std::unique_lock<std::mutex> lm(guard_);
    qu_.push(std::move(data));
}

template <typename T>
bool block_queue<T>::try_push(const T&& data) {
    std::unique_lock<std::mutex> lm(guard_);
    if (qu_.size() == capacity_) {
        return false;
    } else {
        qu_.push(std::move(data));
        return true;
    }
}

template <typename T>
std::optional<T> block_queue<T>::pop(T& data) {
    std::unique_lock<std::mutex> lm(guard_);
    data = std::move(qu_.front());
    qu_.pop();
    return data;
}

template <typename T>
bool block_queue<T>::try_pop(T& data) {
    std::unique_lock<std::mutex> lm(guard_);
    if (qu_.empty()) {
        return false;
    } else {
        data = std::move(qu_.front());
        qu_.pop();
        return true;
    }
}

template <typename T>
void block_queue<T>::print_queue() const {
    std::unique_lock<std::mutex> lm(guard_);
    for (int i = 0; i < qu_.size(); ++i) {
        std::cout<<qu_[i]<<std::endl;
    }
}

} // namespace il