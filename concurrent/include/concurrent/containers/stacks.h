#pragma once
#include <mutex>
#include <thread>
#include <iostream>

namespace il {
    template <typename T>
    class stack {
    private:
        T* buf;
        int _top;
        size_t capacity;
    public:
        stack() {}
        stack(size_t size) {
            buf = new T[size];
            capacity = size;
            _top = -1;
        }
        ~stack() {
            delete[] buf;
        }
        stack(const stack<T>& other) {
            capacity = other.capacity;
            _top = other._top;
            buf = new T[capacity];
            for(int i = 0; i < capacity; ++i) {
                buf[i] = other.buf[i];
            }
        }
        stack(const stack<T>&& other) {
            capacity = other.capacity;
            _top = other._top;
            buf = other.buf;
            other.buf = nullptr;
        }
    public:
        T top() {
            return buf[_top--];
        }
        void push(T value) {
            if(isFull()) {
                T* buf_copy = new T[capacity*2];
                for(int i = 0; i < capacity; ++i) {
                    buf_copy[i] = buf[i];
                }
                capacity*=2;
                delete[] buf;
                buf = buf_copy;
            }
            buf[++_top] = value;
        }
        size_t size() {
            return _top + 1;
        }
        bool isEmpty() {
            return _top == -1;
        }
        bool isFull() {
            return _top + 1 == capacity;
        }
    };

    template <typename T, typename MutexType = std::mutex>
    class ThreadSafeStack {
    private:
        MutexType m_;
        stack<T> stack_;
    public:
        ThreadSafeStack() = default;
    public:
        void push(T value) {
            std::unique_lock<MutexType> lock(m_);
            stack_.push(value);
        }
        T top() const {
            std::unique_lock<MutexType> lock(m_);
            return stack_.top();
        }
        size_t size() {
            std::unique_lock<MutexType> lock(m_);
            return stack_.size();
        }
        bool isEmpty() {
            std::unique_lock<MutexType> lock(m_);
            return stack_.isEmpty();
        }
        bool isFull() {
            std::unique_lock<MutexType> lock(m_);
            return stack_.isFull();
        }
    };
}