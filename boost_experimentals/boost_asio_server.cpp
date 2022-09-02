#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <iostream>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <array>


class client_session : public std::enable_shared_from_this<client_session> {
    boost::asio::ip::tcp::socket client_socket_;
    std::array<char, 1024>       buffer_;

public:
    client_session(boost::asio::ip::tcp::socket&& socket)
        : client_socket_(std::move(socket)) { }


    void start() {
        std::cout << "sesion::start()starting reading" << std::endl;
        do_read();
    }
private:

    void do_read() {
        auto self = shared_from_this();
        client_socket_.async_read_some(
            boost::asio::buffer(buffer_),
            [self] (boost::system::error_code ec, size_t bytes_transfered) {
                std::cout << "sesion::start() reading completed" << std::endl;
                std::string str;
                std::copy(self->buffer_.begin(), self->buffer_.end(), std::back_inserter(str));
                std::cout << str << std::endl;
                self->on_read(ec, bytes_transfered);
            }
        );
    }

    void on_read(boost::system::error_code& ec, size_t bytes_transfered) {
        if (ec) { return; }
        if ( !bytes_transfered ) { do_read(); }
        auto self = shared_from_this();
        std::cout << "sesion::start()starting writing" << std::endl; 
        client_socket_.async_write_some(
            boost::asio::buffer(buffer_, bytes_transfered),
            [self] (boost::system::error_code ec, size_t bytes_transferd) {
                std::cout << "sesion::start() writing completed" << std::endl; 
                self->on_write(ec, bytes_transferd);
            }
        );
    }

    void on_write(boost::system::error_code& ec, size_t bytes_transfered) {
        if (ec) { return; }
        do_read();
    }

    void close_connection() {
        client_socket_.close();
    }
};

class server {
    boost::asio::io_context&            io_context_;
    boost::asio::ip::tcp::acceptor      acceptor_;

public:

    server(boost::asio::io_context& context, boost::asio::ip::tcp::endpoint& ep)
        : io_context_(context)
        , acceptor_(io_context_, ep ) {
            do_accept();
        }

private:

    void do_accept() {
        acceptor_.async_accept(
            [&] (boost::system::error_code ec, boost::asio::ip::tcp::socket client_socket) {
                std::cout << "accepting client." << std::endl;
                if (!ec) {
                    std::cout << "Preparing client." << std::endl;
                    std::make_shared<client_session>(std::move(client_socket))->start();
                }
                do_accept();
            });
    }
};




int main() {

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 5000);
    server s(io_context, ep);
    std::cout << "server is created" << std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(10));

    io_context.run();


    return 0;
}