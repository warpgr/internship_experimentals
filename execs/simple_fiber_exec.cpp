#include <iostream>
#include <concurrent/fiber/fiber.hpp>




int main(int argc, char** argv) {
    auto fiber_1 = il::fiber::fiber::create(
        [] () {
            std::cout << "lambda(): In First fiber before yield()." << std::endl;
            il::fiber::fiber::yield();
            std::cout << "lambda(): In First fiber after yield()." << std::endl;
        },
        "First fiber"
    );

    auto fiber_2 = il::fiber::fiber::create(
        [] () {
            std::cout << "lambda(): In Second fiber before yield()." << std::endl;
            il::fiber::fiber::yield();
            std::cout << "lambda(): In Second fiber after yield()." << std::endl;
        },
        "Second fiber"
    );

    auto fiber_3 = il::fiber::fiber::create(
        [] () {
            std::cout << "lambda(): In Third fiber before yield()." << std::endl;
            il::fiber::fiber::yield();
            std::cout << "lambda(): In Third fiber after yield()." << std::endl;
        },
        "Third fiber"
    );

    std::cout << "main(): In main fiber before yiled()." << std::endl;
    il::fiber::fiber::yield();
    std::cout << "main(): In Main fiber between yield()." << std::endl;
    il::fiber::fiber::yield();
    std::cout << "main(): In Main fiber after yield()." << std::endl;


    int x = 0;
    auto fibre_4 = il::fiber::fiber::create([&x]() { ++x; }, "Fourth fiber");

    std::cout << "main(): Main fiber before yield()." << std::endl;
    std::cout << "main(): x = " << x << std::endl;
    il::fiber::fiber::yield_to(fibre_4);
    std::cout << "main(): Main fiber after yield()." << std::endl;
    std::cout << "main(): x = " << x << std::endl;



    return 0;
}
