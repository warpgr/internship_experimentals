#pragma once
#include <concurrent/fiber/thread_pool.hpp>

#include <concurent/thread_pool.hpp>
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

class socket {
    io_context& context_;
    endpoint    endpoint_;
    int         listenfd_;
public:

    socket(io_context& context, const char* address, uint8_t port)
    : context_(context) {
    , endpoint_(address, port)

    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        std::cerr << "Error opening socket\n";
        std::exit(1); // TODO: handle on other way
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

    void async_read(std:array<uint8_t, 1024>& buffer) {
        context_.post(
            [&] () {
                c_style_read();
            }
        )
    }

    void async_write(std:array<uint8_t, 1024>& buffer) {
        size_t bytes_transfered = 0;
        context_.post(
            [&] () {
                bytes_transfered = c_style_write(buffer);
            }
        );
    }


private:
    size_t c_style_read(const char** buffer) {
        while ( true ) {
            bzero(*buffer, MAXLINE);
            size_t bytes_transfered = recv(listenfd_, *buffer);
            if (bytes_transfered <= 0) {
                fiber::yield();
            }
            return bytes_transfered;
        }
    }

    size_t c_style_write(const char** buffer) {
        send(listenfd_, *buffer);
    }
};


}}
