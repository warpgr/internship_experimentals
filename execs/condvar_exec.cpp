// #include <condition_variable>

#include <concurrent/condition_variable.hpp>
#include <concurrent/mutex.hpp>
#include <concurrent/unique_lock.hpp>
#include <string>
#include <thread>
#include <iostream>



using namespace std::chrono_literals;


il::condition_variable<il::unique_lock<il::mutex>> cv;
il::mutex                                          m;
int                                                counter;
bool                                               ready = false;


void producer() {
    while (counter < 20) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        il::unique_lock<il::mutex> lock(m);
        counter++;
        cv.notify_one();
    }
}

void consumer() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        il::unique_lock<il::mutex> lock(m);
        if (cv.wait_for(lock, std::chrono::seconds(3))) {
            std::cout << "Geting notification counter is " << counter << std::endl;
        }
        else {
            std::cout << "Not Geting yet notification counter is " << counter << std::endl;
        }
    }
}

void wait_for_test() {
    bool ready = false;
    std::thread producer_thread {producer};
    std::thread consumer_thread {consumer};

    producer_thread.join();
    consumer_thread.join();
}


void producer_pred() {
    while (counter < 20) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        il::unique_lock<il::mutex> lock(m);
        counter++;
        cv.notify_one();
    }
}

bool pred() {
    std::cout << "pred called readi " << std::to_string(ready) << std::endl; 
    return ready;
}

void consumer_pred() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        il::unique_lock<il::mutex> lock(m);
        if (cv.wait_for(lock, std::chrono::seconds(3)),
            pred) {
            std::cout << "Geting notification counter is " << counter << std::endl;
        }
        else {
            std::cout << "Not Geting yet notification counter is " << counter << std::endl;
        }
    }
}

void wait_for_test_with_pred() {
    std::thread producer_thread {producer_pred};
    std::thread consumer_thread {consumer_pred};

    producer_thread.join();
    consumer_thread.join();
}


int main() {
    wait_for_test_with_pred();
    return 0;
}




/*
il::condition_variable<il::unique_lock<il::mutex>> cv;
il::mutex cv_m;
int i;
 
void waits(int idx)
{
    il::unique_lock<il::mutex> lk(cv_m);
    if(cv.wait_for(lk, std::chrono::seconds(idx * 1), [] (){return i == 1;})) 
        std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    else
        std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
}
 
void signals()
{
    std::this_thread::sleep_for(1200ms);
    std::cerr << "Notifying...\n";
    cv.notify_all();
    std::this_thread::sleep_for(1000ms);
    {
        il::lock_guard<il::mutex> lk(cv_m);
        i = 1;
    }
    std::cerr << "Notifying again...\n";
    cv.notify_all();
}
 
int main()
{
    std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
*/


/*
il::mutex m;
il::condition_variable<il::unique_lock<il::mutex>> cv;
std::string data;
bool ready = false;
bool processed = false;
 
void worker_thread()
{
    // Wait until main() sends data
    il::unique_lock<il::mutex> lk(m);
    cv.wait(lk, []{return ready;});
 
    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";
 
    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";
 
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}
 
int main()
{
    std::thread worker(worker_thread);
 
    data = "Example data";
    // send data to the worker thread
    {
        il::lock_guard<il::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();
 
    // wait for the worker
    {
        il::unique_lock<il::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }
    std::cout << "Back in main(), data = " << data << '\n';
 
    worker.join();
}
*/