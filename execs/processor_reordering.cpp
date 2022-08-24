#include <thread>
#include <iostream>

void compiler_barier() {
    std::cout << "compiler_barier" << std::endl;
    asm volatile ("");
}

void processor_barier() {
    std::cout << "processor_barier" <<std::endl;
    asm volatile ("mfence");
}



void processor_reordring(int iter) {

    int x = 0;
    int y = 0;

    int r1 = 0;
    int r2 = 0;

    std::thread th_1 {
        [&] ( ) {
            x = 1;
            processor_barier();
            r1 = y;
        }
    };
    std::thread th_2 {
        [&] ( ) {
            y = 1;
            processor_barier();
            r2 = x;
        }
    };

    th_1.join();
    th_2.join();

    if (r1 == 0 && r2 == 0) {
        std::cout << "Broeken CPU on iteration " << iter << std::endl;
        exit(1);
    }
}

int main() {

    for(int i = 0; ; ++i) {
        processor_reordring(i); 
    }

    return 0;
}