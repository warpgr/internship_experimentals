#include <mutex>
#include <atomic>

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

struct adopt_lock_t {};

template <typename Mutex>
class lock_guard {
private:
    Mutex& m;
public:
    explicit lock_guard(Mutex& _m) : m (_m) { m.lock(); }
    lock_guard(Mutex& _m, adopt_lock_t) : m (_m) {};
    ~lock_guard() { m.unlock(); }
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
};

}
