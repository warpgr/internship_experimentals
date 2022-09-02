#include <concurrent/async.hpp>

#include <chrono>
#include <iostream>
int foo(int value) {
    std::cout << "Passed " << value << " sdad " << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return value + 1;
}

int main() {
    std::cout << "Main tid " << std::this_thread::get_id() << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    auto val =  async_(
        [] () -> int {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // std::cout << "setter thread after sleeping\n";
            // std::cout << std::this_thread::get_id() << std::endl;
            return 1;
        }
    );
    std::cout << "Main" << std::endl;
    int b = await_(val);
    std::cout << b << std::flush;


    int a = 4;

    auto res_fut = async_(
        [] () -> int {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // std::cout << "setter thread after sleeping\n";
            std::cout << std::this_thread::get_id() << std::endl;
            return 1;
        }
    )
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo)
    .then(foo);

    // auto res_fut_2 = async_(
    //     [] () -> int {
    //         std::this_thread::sleep_for(std::chrono::seconds(2));
    //         // std::cout << "setter thread after sleeping\n";
    //         std::cout << std::this_thread::get_id() << std::endl;
    //         return 10;
    //     }
    // )
    // .then(foo)
    // .then(foo)
    // .then(foo)
    // .then(foo)
    // .then(foo);

    // std::cout << "Main" << std::endl;
    // // std::this_thread::sleep_for(std::chrono::seconds(2));
    // std::cout << "Final res 1 " << await(res_fut) << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    // std::cout << "Final res 2 " << await(res_fut_2) << std::endl;
    // // .then([] (int val) -> int { return 2; });
    return 0;
}