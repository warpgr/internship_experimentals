#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <shared_mutex>

namespace il {
    template <typename MutexType = shared_mutex>
    class shared_lock {
    private:
        MutexType* m_ = nullptr;
    public:
        shared_lock(MutexType& m) {
           m_ = m;
           assert(m_ != nullptr);
           m_->lock_shared(); 
        }
        ~shared_lock() {
            m_->unlock_shared();
        }
        shared_lock&& operator=(shared_lock&& other) {
            m_(other.m_);
            other.m_ = nullptr;
            return std::move(*this);
        }
        shared_lock(const shared_lock& other) = delete;
        shared_lock& operator=(const shared_lock& other) = delete;
        shared_lock(shared_lock&& other) = delete;
    public:
        void lock() {
            assert(nullptr != m_);
            m_->lock();
        }
        void unlock() {
            assert(nullptr != m_);
            m_->unlock();
        }
    };
}

                              // -- Testing -- //
// class ThreadSafeCounter {
//  public:
//   ThreadSafeCounter() = default;
 
//   // Multiple threads/readers can read the counter's value at the same time.
//   unsigned int get() const {
//     std::shared_lock lock(mutex_);
//     return value_;
//   }
 
//   // Only one thread/writer can increment/write the counter's value.
//   unsigned int increment() {
//     std::unique_lock lock(mutex_);
//     return ++value_;
//   }
 
//   // Only one thread/writer can reset/write the counter's value.
//   void reset() {
//     std::unique_lock lock(mutex_);
//     value_ = 0;
//   }
 
//  private:
//   mutable il::shared_mutex mutex_;
//   unsigned int value_ = 0;
// };
 
// int main() {
//   ThreadSafeCounter counter;
 
//   auto increment_and_print = [&counter]() {
//     for (int i = 0; i < 3; i++) {
//       std::cout << std::this_thread::get_id() << ' ' << counter.increment() << '\n';
 
//       // Note: Writing to std::cout actually needs to be synchronized as well
//       // by another std::mutex. This has been omitted to keep the example small.
//     }
//   };
 
//   std::thread thread1(increment_and_print);
//   std::thread thread2(increment_and_print);
 
//   thread1.join();
//   thread2.join();
// }