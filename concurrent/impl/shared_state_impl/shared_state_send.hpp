#pragma once 
#include <shared_state_impl/shared_state_interface.hpp>


namespace il { namespace impl {

template <typename T, typename ConditionVariabletype, Mutex mutex_type>
class shared_state_send {
    std::shared_ptr<shared_state<T, ConditionVariabletype, mutex_type>> state_ = { nullptr };
public:
    shared_state_send() { state_ = std::make_shared<shared_state<T, ConditionVariabletype, mutex_type>>(); }
    shared_state_send(const std::shared_ptr<shared_state<T, ConditionVariabletype, mutex_type>>& state) { state_ = state; }
    shared_state_send(std::shared_ptr<shared_state<T, ConditionVariabletype, mutex_type>>&& state) { state_ = std::move(state); }

    shared_state_send(const shared_state_send& other) = delete;
    shared_state_send(shared_state_send&& other) = delete;
    shared_state_send& operator=(const shared_state_send& other) = delete;
    shared_state_send&& operator=(shared_state_send&& other) = delete;
public:
    void set_value(T& value) {
        have_correct_state(state_);
        state_->set_value(std::forward<T>(value));
    }
    void set_value(T&& value) {
        have_correct_state(state_);
        state_->set_value(std::forward<T>(value));
    }
    void set_value(const T& value) {
        have_correct_state(state_);
        state_->set_value(std::forward<T>(value));
    }
    void set_value() {
        have_correct_state(state_);
        state_->set_value(std::forward<T>(T()));
    }
    void set_exception(const std::exception& ex) {
        have_correct_state(state_);
        state_->set_exception(std::forward<T>(ex));
    }
    void set_exception(std::exception&& ex) {
        have_correct_state(state_);
        state_->set_exception(std::forward<T>(ex));
    }
    std::shared_ptr<shared_state<T, ConditionVariabletype, mutex_type>> state() {
        return state_;
    }
    void set_callback(std::function<void()> callback) {
        state_->set_callback(callback);
    }
};
}}