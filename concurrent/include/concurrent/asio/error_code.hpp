#pragma once 
#include <cstring>
#include <string>

namespace il { namespace asio {


enum error_number {
    socket_open_error       = 0,
    socket_bind_error       = 1,
    socket_listen_error     = 2,
    socket_connect_error    = 3,
    socket_read_error       = 4,
    socket_write_error      = 5
};


class error_code {
char* message_;
error_number err_num_;

public:
    error_code(error_number en, const char* message)
        : err_num_(en) {
        size_t msg_size = strlen(message);
        message_ = new char(msg_size);
        memcpy(message_, message, msg_size);
    }

    error_code(const error_code& ec) = delete;
    error_code& operator=(const error_code& ec) = delete;

    error_code(error_code&& ec)
        : message_(std::move(ec.message_))
        , err_num_(std::move(ec.err_num_)) { }

    error_code&& operator=(error_code&& ec) {
        delete[] message_;
        message_ = std::move(ec.message_);
        err_num_ =std::move(ec.err_num_);
        return std::move(*this);
    }

    std::string message() const {
        return std::string(message_);
    }

    error_number code() const {
        return err_num_;
    }

    ~error_code() {
        delete[] message_;
    }


}


}}