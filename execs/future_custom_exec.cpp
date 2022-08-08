#include <concurrent/future.hpp>






int main() {

    il::promise<int> p;
    auto f = p.get_future();


    return 0;
}