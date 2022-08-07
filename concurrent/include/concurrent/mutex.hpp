#include <mutex>
#include <atomic>
#include <concepts>

#pragma once

namespace il {

class mutex {
private:
    std::atomic<bool> captured {false};
public:
    mutex() { }
    mutex (mutex&& other) = delete;
    mutex&& operator=(mutex&& other) = delete;
    mutex(const mutex& other) = delete;
    mutex& operator=(const mutex& other) = delete;
    void lock();
    void unlock();
    bool try_lock();
};

template <typename T>
concept Lockable = requires(T m) {
    { m.lock()     };
    { m.unlock()   };
    { m.try_lock() };
};

template <typename T>
concept Mutex = 
    Lockable<T>                   &&
    std::default_initializable<T> &&
    std::destructible<T>          &&
    !std::movable<T>              &&
    !std::copyable<T>;

struct adopt_lock_t {};

template <Mutex mutex_type>
class lock_guard {
private:
    mutex_type& m;
public:
    explicit lock_guard(mutex_type& _m) : m (_m) { m.lock(); }
    lock_guard(mutex_type& _m, adopt_lock_t) : m (_m) {};
    ~lock_guard() { m.unlock(); }
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

};

}
