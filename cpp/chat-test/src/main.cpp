//============================================================================
// Name        : main.cpp
// Author      : HL
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <signal.h>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

#include "util/thread_pool.hpp"
using namespace chat::util;

static bool running = true;

void onInterrupt(int value) {
    cout << "[onInterrupt] (" << value << ")" << endl;
    running = false;
}



int main(int argc, char* argv[]) {
    // 为优雅关闭做准备
    signal(SIGINT, onInterrupt);
    signal(SIGQUIT, onInterrupt);
    signal(SIGTSTP, onInterrupt);

    thread_pool pool(5);

    pool.execute([](){
        cout << "task~~" << endl;
    });

    cout << "main::execute " << endl;



//    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 5; ++i) {
        pool.execute([i](){
            cout << "task_" << i << endl;
        });
    }
//
//    std::thread(std::bind(&wakeup, pool, 5));

    std::this_thread::sleep_for(std::chrono::seconds(3));
    pool.shutdown();

//    pool.wait();


    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    return 0;
}