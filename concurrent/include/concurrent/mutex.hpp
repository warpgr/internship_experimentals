#pragma once

#include <synchronization_primitives/mutex_impl.hpp>
#include <concepts>
#include <concurrent/handler.hpp>
namespace il {

using mutex = impl::mutex_impl<impl::ThreadBlockingHandler>;

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
