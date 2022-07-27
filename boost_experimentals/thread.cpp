
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/chrono/time_point.hpp>
#include <chrono>
#include <iostream>
using bt = boost::thread;
using namespace std::chrono_literals;

int main(int argc, char** argv) {

    bt th_1{
        [&] () {
            boost::this_thread::sleep_for(boost::chrono::seconds(12));
        }
    };


    if (th_1.joinable()) {
        auto start = boost::chrono::system_clock::now();
        start += boost::chrono::seconds(10);
        bool is_joined = th_1.try_join_until(start);
        if (is_joined) {
            std::cout << "Joined " << std::endl;
        }
        else {
            std::cout << "Can`t join timer expired" << std::endl;
        }
    }

    return 0;
}