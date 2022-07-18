#pragma once


namespace my {

template <typename T>
class allocator {
public:
    allocator() = default;

    T* allocate(size_t element_count) {
        return reinterpret_cast<T*>(new uint8_t[element_count * sizeof(T)]);
    }

    void deallocate(T* ptr) {
        delete[] reinterpret_cast<uint8_t*>(ptr);
    }

    template <typename... Args>
    void construct(T* ptr, Args... args) {
        new (ptr) T(args...);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }
};

template <typename T, typename Alloc = allocator<T>>
class vector {
    size_t size_ = { 0 }, capacity_ = { 10 };
    T* buffer_;
    Alloc allocator_;

public:
    vector(size_t capacity = 10, Alloc aloc = allocator<T>())
        : capacity_(capacity)
        , allocator_(aloc) {
        buffer_ =  allocator_.allocate(capacity_);
    }

    template <typename... Args>
    void emplace_back(Args... args) {
        if (size_ == capacity_) {
            reserve();
        }
        allocator_.construct(buffer_ + size_, args...);
        size_++;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve();
        }
        allocator_.construct(buffer_ + size_, value);
        size_++;
    }

    void pop_back() {
        allocator_.destroy(buffer_ + size_);
        size_--;
    }

    const T& operator[](size_t index) const {
        return buffer_[index];
    }

    void reserve() {
        capacity_ *= 2;
        T* tmp_ptr = buffer_;
        buffer_ = allocator_.allocate(capacity_);
        for (int i = 0; i < size_; ++i) {
            allocator_.construct((buffer_ + i), T(tmp_ptr[i]));
        }
        for (size_t i = 0; i < size_; ++i) {
            allocator_.destroy((tmp_ptr + i));
        }
        allocator_.deallocate(tmp_ptr);
        tmp_ptr = nullptr;
    }
};
}
