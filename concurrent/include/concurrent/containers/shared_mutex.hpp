#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <shared_mutex>

namespace il {
    class shared_mutex {
    private:
        std::atomic<bool> captured_exclusive{false};
        std::atomic<bool> captured_shared{false};
    public:
        shared_mutex (shared_mutex&& other) = delete;
        shared_mutex&& operator=(shared_mutex&& other) = delete;
        shared_mutex(const shared_mutex& other) = delete;
        shared_mutex& operator=(const shared_mutex& other) = delete;
        shared_mutex() = default;
    public: // Exclusive Locking
        void lock() {
            while(captured_shared != false);
            while(captured_exclusive.exchange(true));
        }
        bool try_lock() {
            return !(captured_exclusive.exchange(true));
        }
        void unlock() {
            captured_exclusive = false;
        }
    public: // Shared Locking
        void lock_shared() {
            while(captured_exclusive != false) {}
            captured_shared = true;
        }
        // bool try_lock_shared() { //...
        // }
        void unlock_shared() {
            captured_shared = false;
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