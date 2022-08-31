#pragma once
#include <concurrent/fiber/thread_pool.hpp>

#include <concurent/thread_pool.hpp>

#include <concurrent/asio/error_code.hpp>
#include <sys/socket.h>
#include <array>

namespace il { namespace asio {



class io_context {
    fiber::thread_pool<fiber::fiber_executor> tp_;

public:

    template <typename FuncType, typename... Args>
    void post(FuncType&& func, Args... args) {
        tp_.put_task(std::forward<FuncType>(func), std::forward<Args>(args)...);
    }


// thread_pool
// executor (fiber)
// async io sockets

};

class endpoint {
    sockaddr_in c_socket_;
public:
    endpoint(const char* address, uint8_t port ) {
        bzero((char *) &c_socket_, sizeof(c_socket_));
        c_socket_.sin_family = AF_INET;
        c_socket_.sin_addr.s_addr = inet_addr(address.c_str()); // IPv4 address
        c_socket_.sin_port = htons(port);
    }

    sockaddr_in get() {
        return c_socket_;
    }
};

struct buffer {
    std::array<uint8_t, 1024> buffer_;
};

struct error_code {
    
};



class socket {
    io_context& context_;
    endpoint    endpoint_;
    int         listenfd_;
public:

    socket(io_context& context, const char* address, uint8_t port, error_code& ec)
    : context_(context) {
    , endpoint_(address, port)

    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        ec = error_code(error_number::socket_open_error, "Socket creation error.");
    }
    void bind() {
        if (bind(listenfd, (sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
            std::cerr << "ERROR on binding\n";
            exit(1);// TODO: handle on other way
        }
    }

    void listen() {
        std::cout << "server ip : " << inet_ntoa(servAddr.sin_addr) << std::endl;
        if (listen(listenfd, 5) < 0) {
            std::cerr << "ERROR on listen\n";
            exit(1);// TODO: handle on other way
        }
    }

    void connect() {

    }

    void accept() {

    }

    void async_read(std:array<uint8_t, 1024>& buffer, error_code& ec) {
        context_.post(
            [&] () {
                c_style_read(buffer, ec);
            }
        )
    }

    void async_write(std:array<uint8_t, 1024>& buffer, error_code& ec) {
        size_t bytes_transfered = 0;
        context_.post(
            [&] () {
                bytes_transfered = c_style_write(buffer, ec);
            }
        );
    }


private:
    size_t c_style_read(const char** buffer, error_code& ec) {
        while ( true ) {
            bzero(*buffer, MAXLINE);
            ssize_t bytes_transfered = read(listenfd_, *buffer, SSIZE_MAX);
            switch (bytes_transfered) {
            case 0:
                fiber::yield();
                break;
            case -1:
                ec = error_code(error_number::socket_read_error, "Socket read error.");
                break;
            }
            return bytes_transfered;
        }
    }

    size_t c_style_write(const char** buffer, error_code& ec) {
        ssize_t bytes_transferd = write(listenfd_, *buffer, SSIZE_MAX);
        if ( bytes_transferd < 0 ) {
            ec = error_code(error_number::socket_write_error, "Socket read error.");
        }
    }
};


}}
