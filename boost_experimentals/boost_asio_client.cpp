#include <boost/asio.hpp>
#include <iostream>
#include <array>

class client : public std::enable_shared_from_this<client>{
    boost::asio::io_context&        io_context_;
    boost::asio::ip::tcp::socket    socket_;
    boost::asio::ip::tcp::endpoint  endpoint_;
    std::array<char, 1024>          buf_;

public:

    client(boost::asio::io_context& context, boost::asio::ip::tcp::endpoint ep)
        : io_context_(context)
        , socket_(context)
        , endpoint_(ep) {
    }


    void start_connnection() {
        auto self = shared_from_this();
        socket_.async_connect(endpoint_,
        [self] (boost::system::error_code ec) {
            self->on_connect(ec);
        });
    }
private:
    void on_connect(boost::system::error_code ec) {
        if ( ec) { return; }
        auto self = shared_from_this();
        socket_.async_write_some(boost::asio::buffer("Hello"),
            [self] (boost::system::error_code ec, size_t bytes_transfered) {
                self->on_write(ec, bytes_transfered);
            });
    }

    void on_write(boost::system::error_code ec, size_t bytes_transfered) {
        if ( ec ) { return; }
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(buf_),
            [self] (boost::system::error_code ec, size_t bytes_transfered) {
                std::string str;
                std::copy(self->buf_.begin(), self->buf_.end(), std::back_insert_iterator(str));
                std::cout << str << std::endl;
                self->socket_.close();
            });
    }
};



int main() {


    boost::asio::io_context io_context;
    auto remote_address = boost::asio::ip::make_address("127.0.0.1");

    auto client_ptr = std::make_shared<client>(io_context, boost::asio::ip::tcp::endpoint(remote_address, 5000));
    client_ptr->start_connnection();


    io_context.run();
    return 0;
}