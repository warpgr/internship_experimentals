#include <concurrent/packaged_task.hpp>
 
#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>
#include <string>

// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y) { return std::pow(x,y); }
 
void task_lambda()
{
    il::packaged_task<int(int,int)> task([](int a, int b) {
        return std::pow(a, b); 
    });
    il::future<int> result = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
void task_bind()
{
    il::packaged_task<int()> task(std::bind(f, 2, 11));
    il::future<int> result = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << result.get() << '\n';
}
 
void task_thread()
{
    il::packaged_task<int(int,int)> task(f);
    il::future<int> result = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
 
    std::cout << "task_thread:\t" << result.get() << '\n';
}
 
int main()
{
    task_lambda();
    task_bind();
    task_thread();
}