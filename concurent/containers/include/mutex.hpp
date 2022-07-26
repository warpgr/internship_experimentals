#include <mutex>
#include <atomic>

#ifndef _LIB__MUTEX_HPP
#define _LIB__MUTEX_HPP

namespace lib {

class mutex {
private:
    std::atomic<bool> captured {false};
public:
    void lock();
    void unlock();
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

#endif /* _LIB__MUTEX_HPP */
